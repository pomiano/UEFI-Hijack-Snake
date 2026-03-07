#undef __STDC_VERSION__
#include <efi.h>   
#include <uefi.h>  
#include "file_utils.h"
#include "config.h"

int main(int argc, char **argv) {
    EFI_STATUS status;

    //checking original loader
    EFI_HANDLE partitionHandleOriginal = findPartition(BackupLoaderW);
    if(partitionHandleOriginal == NULL) {
        printf("CRITICAL ERROR: Backup loader not found\n");
        printf("Press any key...\n");
        getchar();
        return 1;
    }

    //delete fake loader
    EFI_HANDLE partitionHandleFake = findPartition(OriginalLoaderW);
    if(partitionHandleFake != NULL){
        EFI_FILE_PROTOCOL* root = openRoot(partitionHandleFake);
        if(root != NULL) {
            status = deleteFile(root,OriginalLoaderW);
            if(EFI_ERROR(status) && status != EFI_NOT_FOUND){
                printf("ERROR: Could not delete fake loader\n");
                printf("INFO: Attempting to rename the file to a fallback name\n");
                printf("Press any key...\n");
                getchar();
                status = renameFile(root, OriginalLoaderW, FallbackLoaderW);
                if(!EFI_ERROR(status)){
                    printf("SUCCESS: The name of the snak gate has been changed\n");
                }else{
                    printf("ERROR: Could not rename fake loader\n");
                    printf("CRITICAL ERROR: System restoration failed\n");
                    printf("Press any key...\n");
                    getchar();
                    root->Close(root);
                    return 1;
                }
            } else {
                printf("SUCCESS: The snake gate has been removed\n");
            }
            root->Close(root);
        }
    }

    //renaming original loader
    EFI_FILE_PROTOCOL* rootOriginal = openRoot(partitionHandleOriginal);
    if(rootOriginal == NULL) return 1;

    status = renameFile(rootOriginal, BackupLoaderW, OriginalLoaderW);
    if(EFI_ERROR(status)) {
        printf("CRITICAL ERROR: Failed to rename original file\n");
        printf("Press any key...\n");
        getchar();
        rootOriginal->Close(rootOriginal);
        return 1;
    }

    printf("SUCCESS: System restored succesfully!\n");
    printf("Press any key...\n");
    getchar();
    rootOriginal->Close(rootOriginal);

    return 0;

} 