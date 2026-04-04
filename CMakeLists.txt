cmake_minimum_required(VERSION 3.22)

project(fanzy C ASM)

message("Build type: ${CMAKE_BUILD_TYPE}")

# Default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Export compile commands (clangd etc)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Linker script
# set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/STM32G030XX_FLASH.ld)

# Create executable
add_executable(${PROJECT_NAME})

# STM32CubeMX sources
add_subdirectory(cmake/stm32cubemx)

# User sources
target_sources(${PROJECT_NAME} PRIVATE
    Core/Src/
    Core/Src/temp.c
    Core/Src/logger.c
)

# Includes
target_include_directories(${PROJECT_NAME} PRIVATE
    Core/Inc
)

# Defines
target_compile_definitions(${PROJECT_NAME} PRIVATE

    # your macros
)

# Proper flags (DON’T use global CMAKE_C_FLAGS)
target_compile_options(${PROJECT_NAME} PRIVATE
    -mcpu=cortex-m0plus
    -mthumb
    -ffunction-sections
    -fdata-sections
)

target_link_options(${PROJECT_NAME} PRIVATE
    # -T${LINKER_SCRIPT}
    -mcpu=cortex-m0plus
    -mthumb
    -Wl,--gc-sections
)

# Link libs
target_link_libraries(${PROJECT_NAME}
    stm32cubemx
)

add_custom_command(TARGET fanzy POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:fanzy> $<TARGET_FILE_DIR:fanzy>/fanzy.bin
    COMMENT "Generating binary..."
)
