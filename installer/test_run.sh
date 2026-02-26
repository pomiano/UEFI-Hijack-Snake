#!/bin/bash

# --- KONFIGURACJA ---
INSTALLER="installer.efi"       # Twój instalator
FAKE_WIN="fake_windows.efi"    # Twoja "ofiara" (udawany Windows)
ESP_IMG="esp.img"               # Nazwa obrazu dysku twardego

# 1. Czyszczenie starych plików i tworzenie nowego obrazu 64MB
echo "--- Przygotowywanie wirtualnego dysku ---"
rm -f "$ESP_IMG"
dd if=/dev/zero of="$ESP_IMG" bs=1M count=64
mkfs.vfat -F 32 "$ESP_IMG"

# 2. Tworzenie struktury folderów na dysku (udajemy instalację Windowsa)
# mtools używa :: jako oznaczenie głównego katalogu obrazu
echo "--- Budowanie struktury folderów EFI ---"
mmd -i "$ESP_IMG" ::/EFI
mmd -i "$ESP_IMG" ::/EFI/Microsoft
mmd -i "$ESP_IMG" ::/EFI/Microsoft/Boot

# 3. Wrzucenie ofiary na dysk pod nazwą oryginalnego loadera
echo "--- Kopiowanie ofiary (fake_windows.efi) na dysk ---"
mcopy -i "$ESP_IMG" "$FAKE_WIN" ::/EFI/Microsoft/Boot/bootmgfw.efi

# 4. Znajdowanie BIOSu UEFI (OVMF)
OVMF=$(find /usr/share -name OVMF.fd -o -name OVMF_CODE.fd | head -n 1)

if [ -z "$OVMF" ]; then
    echo "BŁĄD: Nie znaleziono OVMF.fd! Zainstaluj pakiet ovmf."
    exit 1
fi

# 5. Odpalenie QEMU
echo "--- Uruchamianie QEMU ---"
# -drive format=raw,file="$ESP_IMG"  -> DYSK TWARDY (fs0:)
# -drive format=raw,file=fat:rw:.    -> TWÓJ PENDRIVE (fs1:)
qemu-system-x86_64 \
    -bios "$OVMF" \
    -drive format=raw,file="$ESP_IMG" \
    -drive format=raw,file=fat:rw:. \
    -net none \
    -m 256M