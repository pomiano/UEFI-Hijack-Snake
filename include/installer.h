#ifndef INSTALLER_H
#define INSTALLER_H

#undef __STDC_VERSION__
#include <efi.h>   
#include <uefi.h>  

int MainInstall();
EFI_FILE_PROTOCOL* FindWindowsPartition();
int backup_original();
int install_payload();
EFI_FILE_PROTOCOL* OpenRoot(EFI_HANDLE handle);
EFI_FILE_PROTOCOL* OpenFile(EFI_FILE_PROTOCOL *root, CHAR16 *path, UINT64 mode);
EFI_STATUS ChangeFileName(EFI_FILE_PROTOCOL *file, CHAR16 *newName);
#endif