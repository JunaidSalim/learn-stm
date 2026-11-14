cmake_minimum_required(VERSION 3.22)

project(learn-stm C CXX ASM)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)  # lets VS Code find include paths

if(NOT CMAKE_CROSSCOMPILING)
    message(FATAL_ERROR "Pass -DCMAKE_TOOLCHAIN_FILE=cmake/arm-gcc-toolchain.cmake")
endif()

# Only Libs and other stuff
add_subdirectory(libs)
add_subdirectory(apps)