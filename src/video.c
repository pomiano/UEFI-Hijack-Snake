#include "video.h"


void printString(CHAR16 *String){
    ST->ConOut->OutputString(ST->ConOut, String);
}

void clearScreen(void) {
    ST->ConOut->ClearScreen(ST->ConOut);
}

void resetScreen(BOOLEAN extendedVerification) {
    ST->ConOut->Reset(ST->ConOut, extendedVerification);
}