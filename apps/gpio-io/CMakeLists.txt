add_executable(gpio-io
    Core/Src/main.cpp
    Core/Src/stm32f1xx_it.cpp
    Core/Src/syscalls.c
    Core/Src/sysmem.c
    startup_stm32f103xb.s)

target_include_directories(gpio-io PRIVATE Core/Inc)

target_link_libraries(gpio-io PRIVATE board)

target_link_options(gpio-io PRIVATE
    -T${CMAKE_CURRENT_SOURCE_DIR}/STM32F103XX_FLASH.ld)

set_target_properties(gpio-io PROPERTIES SUFFIX .elf)
