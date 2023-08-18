# Simple Kernel Module for Linux Procfs

## Introduction

Welcome to the Simple Kernel Module for Linux Procfs repository! This project provides an example of a basic kernel module that interacts with the procfs (proc filesystem) in the Linux kernel. The module allows you to create, read from, and write to a virtual file within the proc filesystem.

This README will guide you through the process of building, installing, using the module, and cleaning up after use. Whether you're a newcomer to kernel development or an experienced developer looking for a refresher, this project can help you understand the essentials of working with the procfs interface.

## Table of Contents

- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Building the Module](#building-the-module)
- [Installing and Loading](#installing-and-loading)
- [Usage](#usage)
  - [Reading from the Proc File](#reading-from-the-proc-file)
  - [Writing to the Proc File](#writing-to-the-proc-file)
- [Cleaning Up](#cleaning-up)
- [Contributing](#contributing)
- [License](#license)

## Getting Started

### Prerequisites

To work with this kernel module, you'll need:

- A Linux system with kernel development tools installed (gcc, make, headers).
- Basic familiarity with kernel module development concepts.

### Building the Module

1. Clone this repository to your local machine :

   ```bash
   git clone https://github.com/Noureddine-0/Kernel.git

2. Navigate to the project directory :

   ```bash
   cd procfs1

3. Build the kernel module :

   ```bash
   make

## Installing and Loading

1. Load the module into the kernel :

   ```bash
   sudo insmod procfs.ko

2. Check the kernel logs to verify successful loading :

     ```bash
       sudo dmesg | tail

## Usage

### Reading from the Proc File
1. You can read the content of the proc file using any text reader utility. For example :
   
    ```bash
    cat  /proc/procfs

### Writing to the Proc File
1. As root , you can Write to the proc file using echo or any programming language :

    ```bash
    echo Hello World >/proc/procfs

## Cleaning up 

1. To remove the module from the kernel :

    ```bash
    sudo rmmod procfs

## Contributing
Contributions to this project are welcome! If you find issues or want to improve the module, feel free to open a pull request.

## License
This project is licensed under the GNU General Public License v2.0. You are free to modify and distribute the code according to the terms of this license.
