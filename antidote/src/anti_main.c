#undef __STDC_VERSION__
#include <efi.h>   
#include <uefi.h>  
#include "file_utils.h"
#include "config.h"

int main(int argc, char **argv) {

    EFI_STATUS status;

    //checking original loader
    EFI_HANDLE partitionHandleOriginal = findPartition(BackupLoaderW);
    if(partitionHandleOriginal == NULL) return 1;

    //delete fake loader
    EFI_HANDLE partitionHandleFake = findPartition(BackupLoaderW);
    if(partitionHandleFake != NULL){
        EFI_FILE_PROTOCOL* root = openRoot(partitionHandleFake);
        if(root == NULL) return 1;
        status = delete_file(root,OriginalLoaderW);
    }

    //renaming original loader
    EFI_FILE_PROTOCOL* root = openRoot(partitionHandleOriginal);
    if(root == NULL) return 1;

    status = renameFile(root, BackupLoaderW, OriginalLoaderW);
    if(EFI_ERROR(status)) return 1;


    return 0;

} 