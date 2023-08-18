#include <linux/module.h>
#include <linux/init.h>
#include <linux/printk.h>
#include <linux/uaccess.h>
#include <linux/proc_fs.h>
#include <linux/kernel.h>

// Define the name of the proc file
#define PROCFILE  "procfs"
// Define the initial content for the proc file
#define INIT_PHRASE "Initializing\n\0"
// Length of the initial content
#define INIT_PHRASE_LEN 14
// Success return code
#define SUCCESS 0

// Macro to calculate the minimum of two values
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

// Pointer to the proc file entry
static struct proc_dir_entry *our_proc_file;

// Function prototypes for read and write operations
static ssize_t procfile_read(struct file *, char __user *, size_t, loff_t *);
static ssize_t procfile_write(struct file *, const char __user *, size_t, loff_t *);

// Structure for procfs operations
static const struct proc_ops fops = {
    .proc_read = procfile_read,
    .proc_write = procfile_write,
};

// Pointer to the buffer for data storage
static char *buffer;
// Size of the allocated buffer
static int allocated;

// Module initialization function
static int __init initialize(void) {
    // Allocate memory for the buffer
    buffer = kmalloc(INIT_PHRASE_LEN, GFP_KERNEL);
    if (buffer == NULL) {
        return -ENOMEM;
    }
    allocated = INIT_PHRASE_LEN;

    // Initialize buffer with the initial content
    strcpy(buffer, INIT_PHRASE);

    // Create the proc file using proc_create
    our_proc_file = proc_create(PROCFILE, 0644, NULL, &fops);
    if (our_proc_file == NULL) {
        pr_alert("procfs: Could not create the proc file\n");
        return -ENOMEM;
    }

    pr_info("procfs: The procfile %s was created\n", PROCFILE);
    return SUCCESS;
}

// Read operation for the proc file
static ssize_t procfile_read(struct file *file, char __user *buf, size_t size, loff_t *offset) {
    // Calculate the available data length to copy
    size_t slice = MIN(size, allocated - *offset);

    // Check for bounds and copy data to user
    if (*offset >= allocated || copy_to_user(buf, buffer + *offset, slice)) {
        return 0;
    } else {
        *offset += slice;
    }
    return slice;
}

// Write operation for the proc file
static ssize_t procfile_write(struct file *file, const char __user *buf, size_t size, loff_t *offset) {
    // Resize the buffer using krealloc
    char *new_buffer;
    new_buffer = krealloc(buffer, size, GFP_KERNEL);
    if (new_buffer == NULL)
        return -ENOMEM;
    buffer = new_buffer;
    allocated = size;

    pr_info("Trying to write at offset %d\n", (int)*offset);

    // Copy data from user buffer to kernel buffer
    if (copy_from_user(buffer, buf, size))
        return 0;

    // Configure offset later , just take it simple for now 
    *offset = 0;
    return size;
}

// Module cleanup function
static void __exit finalize(void) {
    if (buffer) {
        pr_info("procfs: Freeing memory...\n");
        // Free allocated memory
        kfree(buffer);

        pr_info("procfs: Removing %s...\n", PROCFILE);
        // Remove the proc file entry
        proc_remove(our_proc_file);

        pr_info("procfs: %s Removed\n", PROCFILE);
    } else {
        pr_info("procfs: Exiting the module\n");
    }
}

// Specify the module initialization and cleanup functions
module_init(initialize);
module_exit(finalize);

// Specify the module licensing and author information
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Noureddine-0");
