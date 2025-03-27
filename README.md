# Barebones Hobby Kernel for x86_64
## Requirements for building and running
- clang (working with v19.1.0)
- qemu-system-x86_64, qemu-img
- limine bootloader (from https://github.com/limine-bootloader/limine), files are expected to be installed in their default directories
- parted, mkfs.fat
- gdb (for debugging)

## Building and Running
Output from the kernel will be sent to COM1.

Build the kernel:

    make all
Create a bootable disk image (gpt with fat32):

```make img```
Run the kernel in 

    make run
Run the kernel and connect with gdb:

    make debug
