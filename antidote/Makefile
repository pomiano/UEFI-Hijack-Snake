$(shell ln -sfn ../uefi uefi)

TARGET = antidote.efi
BUILDDIR = build
SRCS = $(wildcard src/*.c) $(wildcard ../common/src/*.c)

CFLAGS = -Iinclude -I../include -I../common/include

include uefi/Makefile