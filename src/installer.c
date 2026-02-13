#include "installer.h"


CHAR16 *OriginalLoader = L"\\EFI\\Microsoft\\Boot\\bootmgfw.efi";
CHAR16 *BackupLoader   = L"\\EFI\\Microsoft\\Boot\\bootmgfw_ms.efi";

int MainInstall() {
    EFI_FILE_PROTOCOL* partition = FindWindowsPartition();
    if(partition == NULL)
        return 1;
    printf("dziala\n");

    return 0;
}

EFI_FILE_PROTOCOL* FindWindowsPartition() {
    EFI_GUID file_guid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
    EFI_HANDLE *handle_buffer;
    UINTN handle_count;
    EFI_STATUS status = BS->LocateHandleBuffer(
        ByProtocol,
        &file_guid,
        NULL,
        &handle_count,
        &handle_buffer
    );
    if(EFI_ERROR(status)) return 1;

    for(int i=0;i<handle_count;i++) {

        EFI_FILE_PROTOCOL *root = OpenRoot(handle_buffer[i]);
        if (!root) continue;
        
        EFI_FILE_PROTOCOL *target = OpenFile(root,OriginalLoader, EFI_FILE_MODE_READ);
        if(target) {
            printf("znaleziono windowsa\n");
            target->Close(target);
            BS->FreePool(handle_buffer);
            return root;
            break;
        }
        root->Close(root);
        
    }
    BS->FreePool(handle_buffer);
    return 0;
}

EFI_FILE_PROTOCOL* OpenRoot(EFI_HANDLE handle) {
    EFI_GUID file_guid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *file_protocol = NULL;
    EFI_FILE_PROTOCOL *root = NULL;

    EFI_STATUS status = BS->OpenProtocol(
        handle,
        &file_guid,
        (void**)&file_protocol,
        IM,
        NULL,
        EFI_OPEN_PROTOCOL_GET_PROTOCOL
    );
    if(EFI_ERROR(status)) return NULL;

    
    status = file_protocol->OpenVolume(
        file_protocol,
        &root
    );
    if(EFI_ERROR(status)) return NULL;

    return root;

}


EFI_FILE_PROTOCOL* OpenFile(EFI_FILE_PROTOCOL *root, CHAR16 *path, UINT64 mode) {
    EFI_FILE_PROTOCOL *target_file = NULL;
    EFI_STATUS status = root->Open(
        root,
        &target_file,
        path,
        mode,
        0
    );
    if(status == EFI_SUCCESS){
        printf("Znaleziono plik\n");
        return target_file;
    } 
    return NULL;

}

EFI_STATUS ChangeFileName(EFI_FILE_PROTOCOL *file, CHAR16 *newName){
    UINTN infoSize = sizeof(EFI_FILE_INFO) + 100;
    EFI_FILE_INFO *info = NULL;

    EFI_STATUS status = BS->AllocatePool(EfiLoaderData,infoSize,(VOID**)&info);
    if (EFI_ERROR(status)) return status;

    EFI_GUID file_guid = EFI_FILE_INFO_GUID;
    status = file->GetInfo(
        file,
        &file_guid,
        &infoSize,
        info
    );
    if (EFI_ERROR(status)) {
        BS->FreePool(info);
        return status;
    }

    for(int i=0; newName[i] != 0; i++) info->FileName[i] = newName[i];

    info->Size = sizeof(EFI_FILE_INFO) + (strlen(newName) * 2);

    status = file->SetInfo(
        file,
        &file_guid,
        &infoSize,
        info
    );

    BS->FreePool(info);
    file->Close(file);
    return status;
}


//entry point 37E80 trzeba za entry pointa wstawic moj, i pozniej skoczyc pod 37E80


int backup_original() {

}

int install_payload() {

}