# Barebones Hobby Kernel for x86_64
## Requirements for building and running
- clang-16 (some C23 features are required)
- qemu-system-x86_64, qemu-img
- limine bootloader (from https://github.com/limine-bootloader/limine), files are expected to be installed in their default directories
- parted, mkfs.fat
- gdb (for debugging)

## Building and Running
Output from the kernel will be sent to COM1.

Build the kernel and a bootable virtual harddisk:

    make all
Run the kernel in qemu:

    make run
Run the kernel and connect with gdb:

    make debug
Run the kernel and provide a gdb stub

    make debug_server
