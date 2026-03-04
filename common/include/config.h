#ifndef CONFIG_H
#define CONFIG_H

#undef __STDC_VERSION__ 
#include <efi.h>   
#include <uefi.h>

// ASCII paths for POSIX-like functions (e.g., fopen)
static const char* const OriginalLoaderA = "\\EFI\\Microsoft\\Boot\\bootmgfw.efi";
static const char* const BackupLoaderA   = "\\EFI\\Microsoft\\Boot\\bootmgfw_ms.efi";

// Wide character paths for native UEFI functions (e.g., Delete, Rename) 
static const CHAR16* const OriginalLoaderW = (const CHAR16*)L"\\EFI\\Microsoft\\Boot\\bootmgfw.efi";
static const CHAR16* const BackupLoaderW   = (const CHAR16*)L"\\EFI\\Microsoft\\Boot\\bootmgfw_ms.efi";


#endif