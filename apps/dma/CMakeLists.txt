add_executable(dma
    Core/Src/main.cpp
    Core/Src/stm32f1xx_it.cpp
    Core/Src/syscalls.c
    Core/Src/sysmem.c
    startup_stm32f103xb.s)

target_include_directories(dma PRIVATE Core/Inc)

target_link_libraries(dma PRIVATE board)

target_link_options(dma PRIVATE
    -T${CMAKE_CURRENT_SOURCE_DIR}/STM32F103XX_FLASH.ld)

set_target_properties(dma PROPERTIES SUFFIX .elf)
