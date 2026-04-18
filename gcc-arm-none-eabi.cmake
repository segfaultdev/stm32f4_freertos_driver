set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# Let user override if needed
set(TOOLCHAIN_PREFIX arm-none-eabi- CACHE STRING "Toolchain prefix")

# Find compilers (cross-platform automatically)
find_program(CMAKE_C_COMPILER   ${TOOLCHAIN_PREFIX}gcc   REQUIRED)
find_program(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}g++   REQUIRED)
find_program(CMAKE_OBJCOPY ${TOOLCHAIN_PREFIX}objcopy   REQUIRED)
find_program(CMAKE_OBJDUMP ${TOOLCHAIN_PREFIX}objdump   REQUIRED)
find_program(CMAKE_SIZE ${TOOLCHAIN_PREFIX}size   REQUIRED)

find_program(STM32CUBE_PRG STM32_Programmer_CLI REQUIRED)

# Prevent try-run (important for embedded)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

message(STATUS "C Compiler: ${CMAKE_C_COMPILER}")
message(STATUS "CXX Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "Objcopy: ${CMAKE_OBJCOPY}")
message(STATUS "Objdump: ${CMAKE_OBJDUMP}")
message(STATUS "Size: ${CMAKE_SIZE}")
message(STATUS "Flaser: ${STM32CUBE_PRG}")

set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/STM32F446RETX_FLASH.ld)
set(STARTUP_FILE ${CMAKE_SOURCE_DIR}/Core/Startup/startup_stm32f446retx.c)

set(CMAKE_C_FLAGS "-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -g -ffunction-sections -fdata-sections -nostdlib")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -fno-rtti -fno-tree-loop-distribution -fno-exceptions")

set(CMAKE_EXECUTABLE_SUFFIX_ASM ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_C ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_CXX ".elf")

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)