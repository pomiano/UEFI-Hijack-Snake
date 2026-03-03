#ifndef BOOTLIB_H
#define BOOTLIB_H

#undef __STDC_VERSION__
#include <efi.h>   
#include <uefi.h>  



EFI_STATUS runEmbeddedSnake(EFI_HANDLE ParentImageHandle);
EFI_STATUS bootFile(EFI_HANDLE ParentImageHandle, char *Path);
#endif 