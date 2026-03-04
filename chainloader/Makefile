$(shell ln -sfn ../uefi uefi)

TARGET = chainloader.efi
BUILDDIR = build
CFLAGS = -Iinclude -I../include -I../common/include

SRCS = $(wildcard src/*.c)

include uefi/Makefile