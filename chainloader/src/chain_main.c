#undef __STDC_VERSION__
#include <efi.h>   
#include <uefi.h>  

#include "bootlib.h"
#include "ui.h"
#include "snake_payload.h"
#include "config.h"

int main(int argc, char **argv) {
    BOOLEAN won = FALSE;
    displayWelcome();
    while(!won){
        if(runEmbeddedSnake(IM) == 0)
            won = TRUE;
    }
    displaySucces();
    bootFile(IM, OriginalLoaderA);

    
    return 0;
}

