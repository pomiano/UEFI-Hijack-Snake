$(shell ln -sfn ../uefi uefi)

TARGET = chainloader.efi
BUILDDIR = build
SRCS = $(wildcard src/*.c)

CFLAGS = -Iinclude -I../include

include uefi/Makefile