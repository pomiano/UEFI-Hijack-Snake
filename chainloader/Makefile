# 1. Tworzymy symlink do frameworka (to już masz, pozwala Makefile'owi niżej działać)
$(shell ln -sfn ../uefi uefi)

# 2. Nazwa pliku wynikowego
TARGET = chainloader.efi

# 3. Ścieżki do nagłówków
# -Iinclude      -> szuka w chainloader/include/
# -I../include   -> szuka w GŁÓWNYM folderze projektu (tam gdzie xxd zapisuje snake_payload.h)
CFLAGS = -Iinclude -I../include

# 4. Pliki źródłowe
SRCS = $(wildcard src/*.c)

# 5. Kluczowa linia: dołączenie logiki budowania UEFI
# To musi być na samym dole, bo ten plik użyje Twoich zmiennych TARGET i SRCS
include uefi/Makefile