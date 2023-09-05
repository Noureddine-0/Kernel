# Linux Kernel Module for File Open Monitoring

This Linux Kernel Module (LKM) is designed to monitor file open operations in the Linux kernel and log them based on user IDs. It provides a way to track which files are being opened by users with specific user IDs.

## Overview

The primary goal of this LKM is to enhance system monitoring and security by logging file open activities. It achieves this by intercepting the `openat` system call and recording the paths of files that are opened by users with a specified User ID (`uid`).

## Features

- Intercepts the `openat` system call in the Linux kernel.
- Logs file open operations, including the file paths.
- Allows filtering file opens by User ID.
- Compatible with Linux kernel versions 5.3.0 and above.

## Module Parameters

This LKM can be configured with the following module parameters:

- `uid`: Specifies the User ID (UID) to match against for logging file opens.
- `address`: Specifies the address of the `sys_call_table`, allowing you to dynamically locate it.

## Usage

1. Load the module using `insmod`:

   ```bash
   sudo insmod your_module.ko uid=1000 address=0xXXXXXXXX
