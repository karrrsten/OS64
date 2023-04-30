SRC = $(shell find . -name '*.[c|s|S]')
OBJ = $(SRC:%=build/%.o)
DEP = $(OBJ:%.o=%.d)
CC_CMD_JSON = $(OBJ:%.o=%.json)

CC = clang-16
QEMU = qemu-system-x86_64

CFLAGS += -g -O0 -std=gnu2x -ffreestanding
CFLAGS += -fms-extensions  -fwrapv -fno-strict-aliasing
CFLAGS += -target x86_64-elf -mgeneral-regs-only -mno-red-zone -mcmodel=large
CFLAGS += -Wall -Wextra -Werror -Wno-microsoft-anon-tag
CFLAGS += -I$(CURDIR) -nostdlib -static

QEMU_FLAGS += -m 512M -machine q35 -cpu max -no-shutdown -no-reboot
QEMU_FLAGS += -d int -M smm=off -trace events=trace_events.cfg -D qemu.log
QEMU_FLAGS += -parallel none -serial stdio -vga none
QEMU_FLAGS += -bios /usr/share/ovmf/OVMF.fd
QEMU_FLAGS += -drive file=$(IMG),if=none,format=raw,id=nvm -device nvme,serial=deadbeef,drive=nvm
#QEMU_FLAGS += -drive file=$(IMG),media=disk,format=raw

IMG = build/os.img
IMG_MOUNT = build/img_mount
KERNEL = build/kernel.elf

.PHONY: all
all: $(IMG) compile_commands.json

# run the kernel in qemu
.PHONY: run
run: all
	$(QEMU) $(QEMU_FLAGS)

# run the kernel and connect with gdb
.PHONY: debug
debug: all
	-killall $(QEMU)
	$(QEMU) $(QEMU_FLAGS) -s -S &
	gdb -q -symbols=$(KERNEL) -ex "target remote localhost:1234"
	-killall $(QEMU)

# run the kernel and provide a gdb stub to connect to
.PHONY: debug-server
debug-server: all
	-killall $(QEMU)
	$(QEMU) $(QEMU_FLAGS) -s -S

# run the kernel and connect with seergdb for debugging
.PHONY: debug-gui
debug-gui:
	-killall $(QEMU)
	$(QEMU) $(QEMU_FLAGS) -s -S &
	seergdb --sym $(KERNEL) --connect localhost:1234
	-killall $(QEMU)


compile_commands.json: $(CC_CMD_JSON)
	sed -e '1s/^/[\n/' -e '$$s/,$$/\n]/' $(CC_CMD_JSON) > compile_commands.json

$(IMG): build limine.cfg $(KERNEL)
	qemu-img create $(IMG) 1G

	parted -s $(IMG) mklabel gpt
	parted -s $(IMG) mkpart esp fat32 2048s 100%
	parted -s $(IMG) set 1 esp on
	limine-deploy $(IMG)

	$(eval LOOP_DEV=$(shell sudo losetup -f))
	sudo losetup $(LOOP_DEV) $(IMG)
	sudo partprobe $(LOOP_DEV)

	sudo mkfs.fat -F 32 $(LOOP_DEV)p1

	sudo mount $(LOOP_DEV)p1 $(IMG_MOUNT)
	sudo mkdir -p $(IMG_MOUNT)/limine
	sudo mkdir -p $(IMG_MOUNT)/EFI/BOOT
	sudo cp $(KERNEL) $(IMG_MOUNT)
	sudo cp limine.cfg /usr/local/share/limine/limine.sys $(IMG_MOUNT)/limine
	sudo cp /usr/local/share/limine/BOOTX64.EFI $(IMG_MOUNT)/EFI/BOOT

	sync
	sudo umount $(IMG_MOUNT)
	sudo losetup -d $(LOOP_DEV)

build:
	$(eval build=$(shell find . -type d \
	! -path './.*' \
	! -path './build' ! -path './build/*'))
	for dir in $(build); do \
		mkdir -p build/$$dir; \
	done
	mkdir -p build/img_mount

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
