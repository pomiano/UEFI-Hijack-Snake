#ifndef INSTALLER_H
#define INSTALLER_H

#undef __STDC_VERSION__
#include <efi.h>   
#include <uefi.h>  

int mainInstall();
EFI_HANDLE findPartition(CHAR16 *fileName);
EFI_FILE_PROTOCOL* openRoot(EFI_HANDLE handle);
EFI_FILE_PROTOCOL* openFile(EFI_FILE_PROTOCOL *root, CHAR16 *path, UINT64 mode);
EFI_STATUS renameFile(EFI_FILE_PROTOCOL *root,CHAR16 *oldName, CHAR16 *newName);
EFI_STATUS writeFileToRoot(EFI_FILE_PROTOCOL *root, CHAR16 *fileName, VOID *buffer, UINTN size);
#endif