TARGET = Snake.efi

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
		-drive file=fat:rw:$(BUILDDIR)/esp,format=raw \
		-vga std \
		-m 256M \
		-net none \
		-device virtio-rng-pci

clean-all: clean
	rm -rf $(BUILDDIR) $(TARGET)