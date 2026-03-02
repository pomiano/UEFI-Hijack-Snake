$(shell ln -sfn ../uefi uefi)

TARGET = fake_windows.efi
BUILDDIR = build
SRCS = $(wildcard src/*.c)

CFLAGS = -Iinclude -I../include

include uefi/Makefile