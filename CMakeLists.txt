cmake_minimum_required(VERSION 3.28)

# Include the ARM toolchan
include(gcc-arm-none-eabi.cmake)

project(freertos LANGUAGES ASM C CXX VERSION 1.0)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)


file(GLOB_RECURSE SOURCES 
    ${CMAKE_SOURCE_DIR}/src/*.c
    ${CMAKE_SOURCE_DIR}/src/*.cpp
    ${CMAKE_SOURCE_DIR}/src/*.cc
)


# Create the main executable target
add_executable(${PROJECT_NAME})


target_sources( ${PROJECT_NAME} PRIVATE
    ${SOURCES}
    ${STARTUP_FILE}
)

# Include directories
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}/Core/CMSIS/Include
    ${CMAKE_SOURCE_DIR}/Core/CMSIS/Device
)


# Compiler definitions
target_compile_definitions(${PROJECT_NAME} PUBLIC STM32F446xx)

# Set compiler options (e.g., enable debugging)
# target_compile_options(${PROJECT_NAME} PUBLIC -g)

# Set linker options
target_link_options(${PROJECT_NAME} PUBLIC 
    -T${LINKER_SCRIPT}
    -Wl,-Map=${PROJECT_NAME}.map
    -Wl,--print-memory-usage
    -Wl,--gc-sections
    -lc 
    -lstdc++
    -lm
)


add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -Obinary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin
)

add_custom_target(flash 
    COMMAND st-flash write ${PROJECT_NAME}.bin 0x08000000
    COMMAND st-flash reset
)