#undef __STDC_VERSION__
#include <efi.h>   
#include <uefi.h>  
#include "installer.h"


CHAR16 *OriginalLoader = L"\\EFI\\Microsoft\\Boot\\bootmgfw.efi";
CHAR16 *BackupLoader   = L"\\EFI\\Microsoft\\Boot\\bootmgfw_ms.efi";

int main(int argc, char **argv) {

    EFI_STATUS status;

    //checking original loader
    EFI_HANDLE partitionHandleOriginal = findPartition(BackupLoader);
    if(partitionHandleOriginal == NULL) return 1;

    //delete fake loader
    EFI_HANDLE partitionHandleFake = findPartition(BackupLoader);
    if(partitionHandleFake != NULL){
        EFI_FILE_PROTOCOL* root = openRoot(partitionHandleFake);
        if(root == NULL) return 1;
        status = delete_file(root,OriginalLoader);
    }

    //renaming original loader
    EFI_FILE_PROTOCOL* root = openRoot(partitionHandleOriginal);
    if(root == NULL) return 1;

    status = renameFile(root, BackupLoader, OriginalLoader);
    if(EFI_ERROR(status)) return 1;


    return 0;

} 