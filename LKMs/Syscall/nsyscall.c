#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/moduleparam.h>
#include <linux/uidgid.h>
#include <linux/version.h>
#include <linux/uaccess.h>
#include <linux/cred.h>

static unsigned long **sys_call_table1;

static unsigned int uid = 1000;  // User ID to match against
module_param(uid, uint, 0644);    // Define a module parameter for 'uid'

static unsigned long address = 0;  // Address of sys_call_table
module_param(address, ulong, 0644); // Define a module parameter for 'address'

/* Prototype of openat in the Linux kernel source code:
   SYSCALL_DEFINE4(openat, int, dfd, const char __user *, filename, int, flags,
       umode_t, mode)
*/

#ifdef CONFIG_ARCH_HAS_SYSCALL_WRAPPER
static asmlinkage long (*original_syscall)(struct pt_regs *);
#else
static asmlinkage long (*original_syscall)(int dfd, const char __user *, int, umode_t);
#endif

// Check the kernel version for compatibility
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,3,0)
static inline void __write_cr0(unsigned long cr0) {
    asm volatile("mov %0, %%cr0" : "+r"(cr0) : : "memory");
}
#else
#define __write_cr0 write_cr0
#endif

// Disable write protection for sys_call_table
static void disable_write_protection(void) {
    unsigned long cr0 = read_cr0();
    clear_bit(16, &cr0);
    __write_cr0(cr0);
}

// Enable write protection for sys_call_table
static void enable_write_protection(void) {
    unsigned long cr0 = read_cr0();
    set_bit(16, &cr0);
    __write_cr0(cr0);
}

#ifdef CONFIG_ARCH_HAS_SYSCALL_WRAPPER
static asmlinkage long our_sys_openat(struct pt_regs *regs)
#else
static asmlinkage long our_sys_openat(int dfd, const char __user *filename, int flags, umode_t mode)
#endif
{
    int i = 0;
    char ch;
    char file_path[256] = {0};

    // Check if the current user's UID matches the specified 'uid'
    if (__kuid_val(current_uid()) != uid)
        goto done;

    pr_info("%s: user with uid %d has opened file:", THIS_MODULE->name, uid);
    do {
        // Get each character of the filename
        #ifdef CONFIG_ARCH_HAS_SYSCALL_WRAPPER
            get_user(ch, (char __user *)regs->si + i);
        #else
            get_user(ch, filename + i);
        #endif

        file_path[i] = ch;
        i++;
    } while (ch != 0);

    // Log the entire file path as a single string
    pr_info("%s\n", file_path);

done:
    #ifdef CONFIG_ARCH_HAS_SYSCALL_WRAPPER
        return original_syscall(regs);
    #else
        return original_syscall(dfd, filename, flags, mode);
    #endif
}

// Module initialization
static int __init my_initializer(void) {
    pr_info("Initializing\n");
    sys_call_table1 = (unsigned long **)address;
    original_syscall = (void *)sys_call_table1[__NR_openat];

    // Disable write protection, hook the openat system call, and enable write protection
    disable_write_protection();
    sys_call_table1[__NR_openat] = (unsigned long *)our_sys_openat;
    enable_write_protection();

    pr_info("Finish initializing\n");
    return 0;
}

// Module cleanup
static void __exit my_finalizer(void) {
    pr_info("%s: Exiting ...\n", THIS_MODULE->name);

    // Disable write protection, restore the original openat system call, and enable write protection
    disable_write_protection();
    sys_call_table1[__NR_openat] = (unsigned long *)original_syscall;
    enable_write_protection();

    pr_info("%s: Exited\n", THIS_MODULE->name);
}

module_init(my_initializer);
module_exit(my_finalizer);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Noureddine-0");
