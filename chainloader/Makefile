TARGET = chainloader.efi

BUILDDIR = build

SRCS = $(wildcard src/*.c)

CFLAGS = -Iinclude

include uefi/Makefile

OVMF = $(shell find /usr/share -name OVMF.fd -o -name OVMF_CODE.fd | head -n 1)

run: $(TARGET)
	@mkdir -p $(BUILDDIR)/esp/EFI/BOOT
	@cp $(TARGET) $(BUILDDIR)/esp/EFI/BOOT/BOOTX64.EFI
	
	@echo "Uruchamianie QEMU..."
	qemu-system-x86_64 \
		-bios $(OVMF) \
		-net none \
		-drive file=fat:rw:$(BUILDDIR)/esp,media=disk,format=raw \
		-drive file=test_disk.img,if=ide,format=raw