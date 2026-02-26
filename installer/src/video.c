#include "video.h"


void clearScreen(void) {
    ST->ConOut->ClearScreen(ST->ConOut);
}

void resetScreen(BOOLEAN extendedVerification) {
    ST->ConOut->Reset(ST->ConOut, extendedVerification);
}