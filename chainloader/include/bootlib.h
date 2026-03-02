#ifndef BOOTLIB_H
#define BOOTLIB_H

#undef __STDC_VERSION__
#include <efi.h>   
#include <uefi.h>  


EFI_FILE_PROTOCOL* openRoot(EFI_HANDLE handle);
EFI_FILE_PROTOCOL* openFile(EFI_FILE_PROTOCOL *root, CHAR16 *path, UINT64 mode);

EFI_STATUS runEmbeddedSnake(EFI_HANDLE ParentImageHandle);
EFI_STATUS bootFile(EFI_HANDLE ParentImageHandle, CHAR16 *Path);
EFI_DEVICE_PATH_PROTOCOL* createFilePath(EFI_HANDLE DeviceHandle, CHAR16 *Path);
EFI_HANDLE findPartition(CHAR16 *fileName);

#endif 