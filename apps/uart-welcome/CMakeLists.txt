add_executable(uart-welcome
    Core/Src/main.cpp
    Core/Src/stm32f1xx_it.cpp
    Core/Src/syscalls.c
    Core/Src/sysmem.c
    startup_stm32f103xb.s)

target_include_directories(uart-welcome PRIVATE Core/Inc)

target_link_libraries(uart-welcome PRIVATE board)

target_link_options(uart-welcome PRIVATE
    -T${CMAKE_CURRENT_SOURCE_DIR}/STM32F103XX_FLASH.ld)

set_target_properties(uart-welcome PROPERTIES SUFFIX .elf)
