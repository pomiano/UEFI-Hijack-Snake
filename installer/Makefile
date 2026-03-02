$(shell ln -sfn ../uefi uefi)

TARGET = installer.efi
BUILDDIR = build
SRCS = $(wildcard src/*.c)

CFLAGS = -Iinclude -I../include

include uefi/Makefile