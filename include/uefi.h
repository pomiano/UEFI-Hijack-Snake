#ifndef UEFI_H
#define UEFI_H

#include <stdint.h>

#define EFIAPI __attribute__((ms_abi))

typedef unsigned long long EFI_STATUS;
typedef void* EFI_HANDLER;
typedef unsigned short CHAR16;
typedef unsigned char BOOLEAN;
typedef uint32_t UINT32;
typedef uint64_t UINT64;


#endif