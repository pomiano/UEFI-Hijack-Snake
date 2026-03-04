$(shell ln -sfn ../uefi uefi)

TARGET = antidote.efi
BUILDDIR = build
SRCS = $(wildcard src/*.c)

CFLAGS = -Iinclude -I../include

include uefi/Makefile