$(shell ln -sfn ../uefi uefi)

TARGET = installer.efi
BUILDDIR = build
SRCS = $(wildcard src/*.c) $(wildcard ../common/src/*.c)

CFLAGS = -Iinclude -I../include -I../common/include

include uefi/Makefile