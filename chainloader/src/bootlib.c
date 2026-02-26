#include "bootlib.h"

extern unsigned char Snake_efi[];
extern unsigned int Snake_efi_len;



EFI_STATUS runEmbeddedSnake(EFI_HANDLE ParentImageHandle) {
    EFI_HANDLE snake_handle = NULL;
    EFI_STATUS status;

    status = BS->LoadImage(
        FALSE,
        ParentImageHandle,
        NULL,
        Snake_efi,
        Snake_efi_len,
        &snake_handle
    );

    if(EFI_ERROR(status)){  return status; }

    UINTN exit_size = 0;
    CHAR16 *exit_data = NULL;

    status = BS->StartImage(snake_handle, &exit_size, &exit_data);

    return status;
}


EFI_STATUS bootFile(EFI_HANDLE ParentImageHandle, CHAR16 *Path) {
    EFI_STATUS status;
    EFI_HANDLE child_handle = NULL;
    EFI_DEVICE_PATH_PROTOCOL *file_path;

    EFI_HANDLE partitionHandle = findPartition(Path);
    if(partitionHandle == NULL) {
        return EFI_NOT_FOUND;
    }
    
    file_path = createFilePath(partitionHandle, Path);
    
    status = BS->LoadImage(
        FALSE,
        ParentImageHandle,
        file_path,
        NULL,
        NULL,
        &child_handle
    );
    if(EFI_ERROR(status)){  return status; }

    status = BS->StartImage(child_handle, NULL, NULL);

    BS->FreePool(file_path);

    return status;
}

EFI_DEVICE_PATH_PROTOCOL* createFilePath(EFI_HANDLE DeviceHandle, CHAR16 *Path){
    EFI_STATUS status;
    EFI_DEVICE_PATH_PROTOCOL *device_path = NULL;
    EFI_DEVICE_PATH_PROTOCOL *full_path = NULL;
    FILEPATH_DEVICE_PATH *file_node = NULL;
    UINTN path_len;

    EFI_GUID device_path_guid = EFI_DEVICE_PATH_PROTOCOL_GUID;

    status = BS->OpenProtocol(
        DeviceHandle,
        &device_path_guid,
        (void**)&device_path,
        NULL,
        NULL,
        EFI_OPEN_PROTOCOL_GET_PROTOCOL
    );
    if(EFI_ERROR(status)) return NULL; 

    path_len = (strlen(Path) + 1) * sizeof(CHAR16);
    status = BS->AllocatePool(EfiLoaderData, sizeof(FILEPATH_DEVICE_PATH) + path_len, (void**)&file_node);
    if (EFI_ERROR(status)) return NULL;

    file_node->Header.Type = MEDIA_DEVICE_PATH;
    file_node->Header.SubType = MEDIA_FILEPATH_DP;
    SetDevicePathNodeLength(&file_node->Header, sizeof(FILEPATH_DEVICE_PATH) + path_len);
    memcpy(file_node->PathName,Path, path_len);

    EFI_DEVICE_PATH_UTILITIES_PROTOCOL *DPUtils = NULL;
    EFI_GUID device_path_utilities_guid = EFI_DEVICE_PATH_UTILITIES_PROTOCOL_GUID;
    status = BS->LocateProtocol(
        &device_path_utilities_guid, 
        NULL, 
        (void**)&DPUtils
    );
    
    if (!EFI_ERROR(status) && DPUtils) {
        full_path = DPUtils->AppendDeviceNode(device_path, &file_node->Header);
    }

    BS->FreePool(file_node);

    return full_path;
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

        EFI_FILE_PROTOCOL *root = OpenRoot(handle_buffer[i]);
        if (!root) continue;
        
        EFI_FILE_PROTOCOL *target = OpenFile(root,fileName, EFI_FILE_MODE_READ);
        if(target) {
            printf("znaleziono windowsa\n");
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