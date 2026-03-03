#include "bootlib.h"

extern unsigned char snake_efi[];
extern unsigned int snake_efi_len;

EFI_STATUS runEmbeddedSnake(EFI_HANDLE ParentImageHandle) {
    EFI_HANDLE snake_handle = NULL;
    EFI_STATUS status;

    status = BS->LoadImage(
        FALSE,
        ParentImageHandle,
        NULL,
        snake_efi,
        snake_efi_len,
        &snake_handle
    );

    if(EFI_ERROR(status)){  return status; }

    UINTN exit_size = 0;
    CHAR16 *exit_data = NULL;

    status = BS->StartImage(snake_handle, &exit_size, &exit_data);

    return status;
}


EFI_STATUS bootFile(EFI_HANDLE ParentImageHandle, char *Path) {
    FILE *file = fopen(Path, "r");
    if (!file) {
        printf("ERROR: Windows doesn't exist...\n");
        getchar();
        return EFI_NOT_FOUND;
    }

    // size of file
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);


    // allocate a pool in RAM to store the entire EFI binary
    void *buffer = malloc(size);
    if (!buffer) {
        fclose(file);
        printf("ERROR: out of RAM!\n");
        getchar();
        return EFI_OUT_OF_RESOURCES;
    }
    
    fread(buffer, 1, size, file);
    fclose(file);

    // load the EFI image into memory
    EFI_HANDLE child_handle = NULL;
    EFI_STATUS status = BS->LoadImage(
        FALSE,
        ParentImageHandle,
        NULL,
        buffer,
        size,
        &child_handle
    );

    if (EFI_ERROR(status)) {
        printf("ERROR: LoadImage: 0x%lx\n", (unsigned long)status);
        getchar();
    } else {
        //transfer control to the loaded EFI image
        status = BS->StartImage(child_handle, NULL, NULL);
    }

    free(buffer);
    return status;
}

