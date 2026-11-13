# learn-stm

Trying to learn the stm with minimal config

# Tip: To build use: cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/arm-gcc-toolchain.cmake .. 
# To flash: STM32_Programmer_CLI --connect port=swd mode=UR --download PATH_TO_ELF_FILE -hardRst -rst --start
# STM32_Programmer_CLI --connect port=swd mode=UR --download ../build/apps/led-blink/led-blink.elf -hardRst -rst --start
# minicom -D /dev/ttyACM0 -b 115200