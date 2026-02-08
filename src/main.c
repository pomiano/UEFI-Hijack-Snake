#undef __STDC_VERSION__
#include <efi.h>   
#include <uefi.h>  
#include "video.h"
#include "video.h"



int main(int argc, char **argv) {

    resetScreen(0);
    printf("Siema\n");
    EFI_TIME mytime;
    RT->GetTime(&mytime,0);
    printf("%d %d %d\n", mytime.Hour, mytime.Minute, mytime.Second);
    printf("%d %d %d\n", mytime.Day, mytime.Month, mytime.Year);
    while(TRUE) {
        getchar();
    }
    return 0;

}