#ifndef VIDEO_H
#define VIDEO_H

#undef __STDC_VERSION__
#include <efi.h>   
#include <uefi.h>

void clearScreen(void);
void resetScreen(BOOLEAN extendedVerification);

#endif