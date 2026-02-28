#undef __STDC_VERSION__
#include <efi.h>   
#include <uefi.h> 

#include "video.h"

int main(int argc, char **argv) {
    resetScreen(0);

    printf("\n\n");
    printf("   ###########################################\n");
    printf("   #                                         #\n");
    printf("   #      WINDOWS BOOT MANAGER (SIMULATED)   #\n");
    printf("   #                                         #\n");
    printf("   ###########################################\n");
    printf("\n");
    printf("   Press any key to continue...\n");

    getchar();
    
    return EFI_SUCCESS;
}