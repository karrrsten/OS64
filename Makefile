SRC = $(shell find . -name '*.[c|s|S]')
OBJ = $(SRC:%=build/%.o)
DEP = $(OBJ:%.o=%.d)
CC_CMD_JSON = $(OBJ:%.o=%.json)

CC = clang
QEMU = qemu-system-x86_64

CFLAGS += -ggdb -g3 -O0 -std=gnu2y -ffreestanding
CFLAGS += -fms-extensions -fwrapv -fno-strict-aliasing #-fstrict-volatile-bitfields
CFLAGS += -target x86_64-elf -mgeneral-regs-only -mno-red-zone -mcmodel=large -fno-stack-protector
CFLAGS += -Wall -Wextra -Werror -Wno-microsoft-anon-tag -Wno-address-of-packed-member -Wno-unused-function
CFLAGS += -I$(CURDIR) -nostdlib -static

QEMU_FLAGS += -m 512M -machine q35 -cpu max -no-shutdown -no-reboot
QEMU_FLAGS += -d int -M smm=off -trace events=trace_events.cfg -D qemu.log
QEMU_FLAGS += -parallel none -serial stdio -vga none
QEMU_FLAGS += -bios /usr/share/ovmf/x64/OVMF.4m.fd
QEMU_FLAGS += -device nvme,serial=deadbeef,drive=nvm

IMG = build/os.img
KERNEL = build/kernel.elf

.PHONY: all
#select whether to run with an image or QEMUs fat emulation
QEMU_FLAGS += -drive file=fat:rw:build/img_dir,if=none,format=raw,id=nvm
all: img_dir compile_commands.json
#QEMU_FLAGS += -drive file=$(IMG),if=none,format=raw,id=nvm
#all: $(IMG) compile_commands.json

# run the kernel in qemu
.PHONY: run
run: all
	$(QEMU) $(QEMU_FLAGS)

# run the kernel and connect with gdb
.PHONY: debug
debug: all
	-killall $(QEMU)
	$(QEMU) $(QEMU_FLAGS) -s -S &
	gdb -q
	-killall $(QEMU)


compile_commands.json: $(CC_CMD_JSON)
	sed -e '1s/^/[\n/' -e '$$s/,$$/\n]/' $(CC_CMD_JSON) > compile_commands.json

img_dir: build limine.conf $(KERNEL)
	mkdir -p build/img_dir
	mkdir -p build/img_dir/limine build/img_dir/EFI/BOOT
	cp $(KERNEL) build/img_dir
	cp limine.conf build/img_dir/limine
	cp /usr/share/limine/BOOTX64.EFI build/img_dir/EFI/BOOT

.PHONY: img
img: $(IMG)
$(IMG): build limine.conf $(KERNEL)
	qemu-img create $(IMG) 1G

	parted -s $(IMG) mklabel gpt
	parted -s $(IMG) mkpart esp fat32 2048s 100%
	parted -s $(IMG) set 1 esp on

	$(eval LOOP_DEV=$(shell sudo losetup -f))
	sudo losetup $(LOOP_DEV) $(IMG)
	sudo partx $(LOOP_DEV)

	sudo mkfs.fat -F 32 $(LOOP_DEV)

	sudo mount -o uid=$(shell id -u) $(LOOP_DEV) build/mnt
	mkdir -p build/mnt/limine
	mkdir -p build/mnt/EFI/BOOT
	cp $(KERNEL) build/mnt
	cp limine.conf build/mnt/limine
	cp /usr/share/limine/BOOTX64.EFI build/mnt/EFI/BOOT

	sudo umount build/mnt
	sudo losetup -d $(LOOP_DEV)

build:
	$(eval build=$(shell find . -type d \
	! -path './.*' \
	! -path './build' ! -path './build/*'))
	for dir in $(build); do \
		mkdir -p build/$$dir; \
	done
	mkdir -p build/mnt

$(KERNEL): build $(OBJ) linker.ld
	$(CC) $(CFLAGS) -T linker.ld -Wl,--build-id=none $(OBJ) -o $@

-include $(DEP)

build/%.o: % | build
	$(CC) -MJ build/$<.json $(CFLAGS) -MMD -c $< -o $@

.PHONY: clean
clean:
	rm -rf build
	rm -rf qemu.log
	rm -rf compile_commands.json
