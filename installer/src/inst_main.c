#undef __STDC_VERSION__
#include <efi.h>   
#include <uefi.h>  
#include "video.h"
#include "installer.h"

#include "chainloader_payload.h"


int main(int argc, char **argv) {

    resetScreen(0);
    mainInstall();

    return 0;

}