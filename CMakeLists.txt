cmake_minimum_required(VERSION 3.28)

include(gcc-arm-none-eabi.cmake)

project(freertos LANGUAGES ASM C CXX VERSION 1.0)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)


file(GLOB_RECURSE SOURCES 
    ${CMAKE_SOURCE_DIR}/src/*.c
    ${CMAKE_SOURCE_DIR}/src/*.cpp
    ${CMAKE_SOURCE_DIR}/src/*.cc
    ${CMAKE_SOURCE_DIR}/driver/src/*.c
)


# Create the main executable target
add_executable(${PROJECT_NAME})


target_sources( ${PROJECT_NAME} PRIVATE
    ${SOURCES}
    ${STARTUP_FILE}
)

# Include directories
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}/core/CMSIS/Include
    ${CMAKE_SOURCE_DIR}/core/CMSIS/Device
    ${CMAKE_SOURCE_DIR}/driver/inc
)


target_compile_definitions(${PROJECT_NAME} PUBLIC STM32F446xx)


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
    DEPENDS ${PROJECT_NAME}
    COMMAND "${STM32CUBE_PRG}"
        -c port=SWD
        mode=UR
        -w $<TARGET_FILE:${PROJECT_NAME}>
        -v -rst
    OUTPUT_QUIET
)