#include "ui.h"

void displayWelcome() {
    ST->ConOut->SetAttribute(ST->ConOut, 0x1F);
    ST->ConOut->ClearScreen(ST->ConOut);
    printf(":(\n\n");

    ST->ConOut->SetAttribute(ST->ConOut, 0x14);
    printf(" [!] CRITICAL SYSTEM ERROR\n\n");

    ST->ConOut->SetAttribute(ST->ConOut, 0x1F);
    printf("Failed to load the Windows Boot Manager (bootmgfw.efi).\n");
    printf("The system partition may be corrupted or inaccessible.\n\n");

    printf("EMERGENCY RECOVERY PROCEDURE:\n");
    printf(" An automated repair sequence has been initialized.\n");
    printf(" To re-synchronize the boot sectors and retry loading the OS,\n");
    printf(" you must complete the data-stream integrity check.\n\n");

    printf("INSTRUCTIONS:\n");
    printf(" In the following module, use the arrow keys to control the snake.\n");
    printf(" Collect the apples to fill the green integrity bar.\n");
    printf(" Reach 10 points to verify and execute the payload.\n\n");

    printf("Press any key to begin recovery...");
    
    getchar();
}

void displaySucces() {
    ST->ConOut->Reset(ST->ConOut,0);

    ST->ConOut->SetAttribute(ST->ConOut, 0x1A);
    printf("\n\n [!] Integrity check PASSED (10/10 apples collected).\n");

    ST->ConOut->SetAttribute(ST->ConOut, 0x1F);
    printf(" [!] Patching boot sectors... OK\n");
    printf(" [!] Synchronizing UEFI variables... OK\n");
    printf(" [!] Verification successful. Executing payload...\n");

    BS->Stall(5000000);
}