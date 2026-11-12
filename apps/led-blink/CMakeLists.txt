add_executable(led-blink
    Core/Src/main.cpp
    Core/Src/stm32f1xx_it.cpp
    Core/Src/syscalls.c
    Core/Src/sysmem.c
    startup_stm32f103xb.s)

target_include_directories(led-blink PRIVATE Core/Inc)

target_link_libraries(led-blink PRIVATE STM32_Drivers)

target_link_options(led-blink PRIVATE
    -T${CMAKE_CURRENT_SOURCE_DIR}/STM32F103XX_FLASH.ld)

set_target_properties(led-blink PROPERTIES SUFFIX .elf)
