#include "uefi.h"


struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

typedef EFI_STATUS (EFIAPI *EFI_TEXT_STRING) (
    struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
    CHAR16 *String
 );

typedef EFI_STATUS (EFIAPI *EFI_TEXT_RESET) (
    struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
    BOOLEAN ExtendedVerification
);

typedef struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL {
    EFI_TEXT_RESET Reset;
    EFI_TEXT_STRING OutputString;
}EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

typedef struct {
  UINT64      Signature;
  UINT32      Revision;
  UINT32      HeaderSize;
  UINT32      CRC32;
  UINT32      Reserved;
 } EFI_TABLE_HEADER;


typedef struct {
  EFI_TABLE_HEADER                 Hdr;
  CHAR16                           *FirmwareVendor;
  UINT32                           FirmwareRevision;
  void* ConsoleInHandle; 
  void* ConIn;      
  void* ConsoleOutHandle;
  EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL  *ConOut;
  void* StandardErrorHandle;
  EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL  *StdErr;
} EFI_SYSTEM_TABLE;


EFI_STATUS EFIAPI efi_main(EFI_HANDLER ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    SystemTable->ConOut->Reset(SystemTable->ConOut, 0);

    SystemTable->ConOut->OutputString(SystemTable->ConOut, (CHAR16*)u"SIEMA!\n");

    while(1) {
        __asm__("hlt");
    }

    return 0;

}