cmake_minimum_required(VERSION 3.10)

project(learn-stm)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

# Tip: To build use: cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/arm-gcc-toolchain.cmake .. 
# To flash: STM32_Programmer_CLI --connect port=swd mode=UR --download PATH_TO_ELF_FILE -hardRst -rst --start

# Only Libs and other stuff
add_subdirectory(libs)
add_subdirectory(apps)