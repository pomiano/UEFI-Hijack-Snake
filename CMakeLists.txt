cmake_minimum_required(VERSION 3.15)
project(UEFIprobe C)

add_executable(UEFIprobe 
    src/main.c
    src/video.c
)

target_include_directories(UEFIprobe PRIVATE include)

# flagi kompilacji
set(UEFI_COMPILE_FLAGS -ffreestanding -fno-stack-protector -fshort-wchar -mno-red-zone -Wall)
target_compile_options(UEFIprobe PRIVATE ${UEFI_COMPILE_FLAGS})

# flagi linkowania
target_link_options(UEFIprobe PRIVATE
    "-nostdlib"
    "-nostartfiles"
    "-Wl,-dll"
    "-Wl,--subsystem,10"
    "-Wl,-e,efi_main"
)

set_target_properties(UEFIprobe PROPERTIES SUFFIX ".efi")

add_custom_target(run
    COMMAND ${CMAKE_COMMAND} -E make_directory esp/EFI/BOOT
    COMMAND ${CMAKE_COMMAND} -E copy UEFIprobe.efi esp/EFI/BOOT/BOOTX64.EFI
    COMMAND qemu-system-x86_64 -bios /usr/share/ovmf/OVMF.fd -drive file=fat:rw:esp,format=raw -net none
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS UEFIprobe
)