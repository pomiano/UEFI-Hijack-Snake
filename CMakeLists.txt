cmake_minimum_required(VERSION 3.15)
project(UEFIprobe C)

add_executable(UEFIprobe src/main.c)

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