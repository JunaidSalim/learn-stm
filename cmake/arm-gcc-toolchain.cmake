set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# Locate the toolchain
if(NOT ARM_TOOLCHAIN_DIR AND DEFINED ENV{ARM_TOOLCHAIN_DIR})
  set(ARM_TOOLCHAIN_DIR "$ENV{ARM_TOOLCHAIN_DIR}")
endif()

# Newest STM32CubeCLT installed on Linux, e.g. /opt/st/stm32cubeclt_1.22.0
file(GLOB CUBECLT_GCC_DIRS LIST_DIRECTORIES true /opt/st/stm32cubeclt_*/GNU-tools-for-STM32/bin)
list(SORT CUBECLT_GCC_DIRS COMPARE NATURAL ORDER DESCENDING)

# Will look at these directories
find_program(ARM_GCC arm-none-eabi-gcc
  HINTS
    "${ARM_TOOLCHAIN_DIR}"
    ${CUBECLT_GCC_DIRS}
    /Applications/ArmGNUToolchain/14.2.rel1/arm-none-eabi/bin
  REQUIRED)

get_filename_component(ARM_BIN  "${ARM_GCC}" DIRECTORY)
get_filename_component(ARM_ROOT "${ARM_BIN}" DIRECTORY)

# Compilers
set(CMAKE_C_COMPILER   "${ARM_BIN}/arm-none-eabi-gcc")
set(CMAKE_CXX_COMPILER "${ARM_BIN}/arm-none-eabi-g++")
set(CMAKE_ASM_COMPILER "${ARM_BIN}/arm-none-eabi-gcc")

# gcc- prefixed wrappers are the LTO-aware ones; keep them if you use -flto
set(CMAKE_AR      "${ARM_BIN}/arm-none-eabi-gcc-ar")
set(CMAKE_RANLIB  "${ARM_BIN}/arm-none-eabi-gcc-ranlib")
set(CMAKE_NM      "${ARM_BIN}/arm-none-eabi-gcc-nm")
set(CMAKE_OBJCOPY "${ARM_BIN}/arm-none-eabi-objcopy" CACHE FILEPATH "")
set(CMAKE_OBJDUMP "${ARM_BIN}/arm-none-eabi-objdump" CACHE FILEPATH "")
set(CMAKE_SIZE    "${ARM_BIN}/arm-none-eabi-size"    CACHE FILEPATH "")

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# TODO: Fix this. It is setting m3 for all boards
set(MCU_FLAGS "-mcpu=cortex-m3 -mthumb")

set(CMAKE_TRY_COMPILE_PLATFORM_VARIABLES TARGET_BOARD ARM_TOOLCHAIN_DIR)

set(CMAKE_C_FLAGS_INIT "${MCU_FLAGS} -ffunction-sections -fdata-sections")
set(CMAKE_CXX_FLAGS_INIT
    "${MCU_FLAGS} -ffunction-sections -fdata-sections \
-fno-exceptions -fno-rtti -fno-threadsafe-statics -fno-use-cxa-atexit")
set(CMAKE_ASM_FLAGS_INIT "${MCU_FLAGS} -x assembler-with-cpp")

set(CMAKE_EXE_LINKER_FLAGS_INIT
    "${MCU_FLAGS} --specs=nosys.specs \
-Wl,--gc-sections -Wl,--print-memory-usage")

set(CMAKE_FIND_ROOT_PATH "${ARM_ROOT}/arm-none-eabi")
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)