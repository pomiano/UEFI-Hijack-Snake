#settings
SNAKE_DIR = snake
INSTALLER_DIR = installer
CHAINLOADER_DIR = chainloader
INCLUDE_DIR = include
BUILD_DIR = build
FAKE_WIN_DIR = fake_win

SNAKE_EFI = $(SNAKE_DIR)/snake.efi
CHAINLOADER_EFI = $(CHAINLOADER_DIR)/chainloader.efi
INSTALLER_EFI = $(INSTALLER_DIR)/installer.efi
FAKE_WIN_EFI = $(FAKE_WIN_DIR)/fake_windows.efi

SNAKE_HEADER = $(INCLUDE_DIR)/snake_payload.h
CHAINLOADER_HEADER = $(INCLUDE_DIR)/chainloader_payload.h


#simulation
ESP_IMG = build/esp.img
DISK_SIZE_MB = 64

.PHONY: all $(SNAKE_EFI) $(CHAINLOADER_EFI) $(FAKE_WIN_EFI)


#rules
run: all $(FAKE_WIN_EFI)
	@echo "Creating virtual disk..."
	@mkdir -p build
	rm -f $(ESP_IMG)
	dd if=/dev/zero of=$(ESP_IMG) bs=1M count=$(DISK_SIZE_MB) status=none
	mkfs.fat -F 32 $(ESP_IMG)

	@echo "Building efi structur..."
	mmd -i $(ESP_IMG) ::/EFI
	mmd -i $(ESP_IMG) ::/EFI/Microsoft
	mmd -i $(ESP_IMG) ::/EFI/Microsoft/Boot
	mcopy -i $(ESP_IMG) $(FAKE_WIN_EFI) ::/EFI/Microsoft/Boot/bootmgfw.efi

	@echo "Launching qemu..."
	qemu-system-x86_64 \
		-bios $(OVMF) \
		-drive format=raw,file=$(ESP_IMG) \
		-drive format=raw,file=fat:rw:$(INSTALLER_DIR) \
		-m 256M -net none


$(FAKE_WIN_EFI):
	$(MAKE) -C $(FAKE_WIN_DIR)


all: $(INSTALLER_EFI)

$(INSTALLER_EFI): $(CHAINLOADER_HEADER)
	$(MAKE) -C $(INSTALLER_DIR)
	
$(CHAINLOADER_HEADER): $(CHAINLOADER_EFI)
	@mkdir -p $(INCLUDE_DIR)
	xxd -i $< > $@

$(CHAINLOADER_EFI): $(SNAKE_HEADER)
	$(MAKE) -C $(CHAINLOADER_DIR)

$(SNAKE_HEADER): $(SNAKE_EFI)
	@mkdir -p $(INCLUDE_DIR)
	xxd -i $< > $@

$(SNAKE_EFI):
	$(MAKE) -C $(SNAKE_DIR)


clean:
	$(MAKE) -C $(SNAKE_DIR) clean
	$(MAKE) -C $(CHAINLOADER_DIR) clean
	$(MAKE) -C $(INSTALLER_DIR) clean
	rm -rf $(INCLUDE_DIR)
	rm -f $(INSTALLER_EFI)