
set(CMAKE_ASM_COMPILER /usr/bin/arm-none-eabi-gcc)
set(CMAKE_C_COMPILER /usr/bin/arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER /usr/bin/arm-none-eabi-g++)
set(CMAKE_OBJCOPY /usr/bin/arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP /usr/bin/arm-none-eabi-objdump)
set(CMAKE_SIZE /usr/bin/arm-none-eabi-size)

set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/STM32F446RETX_FLASH.ld)
set(STARTUP_FILE ${CMAKE_SOURCE_DIR}/Core/Startup/startup_stm32f446retx.c)

set(CMAKE_C_FLAGS "-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -g -ffunction-sections -fdata-sections -nostdlib")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -fno-rtti -fno-tree-loop-distribution -fno-exceptions")

set(CMAKE_EXECUTABLE_SUFFIX_ASM ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_C ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_CXX ".elf")

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)