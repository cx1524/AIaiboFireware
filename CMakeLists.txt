cmake_minimum_required(VERSION 3.20)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

if(NOT CMAKE_C_COMPILER)
    set(CMAKE_C_COMPILER arm-none-eabi-gcc)
endif()

if(NOT CMAKE_ASM_COMPILER)
    set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
endif()

project(AIaiboFirmware LANGUAGES C ASM)

if(NOT CMAKE_C_COMPILER MATCHES "arm-none-eabi-gcc")
    message(FATAL_ERROR "C compiler must be arm-none-eabi-gcc. Use a generator like Ninja and reconfigure.")
endif()

set(FIRMWARE_TARGET "${PROJECT_NAME}.elf")

set(PLACEHOLDER_PREFIX "^__PLACEHOLDER_")

set(MIDDLEWARES_INCLUDE_DIRS
"Middlewares/CMSIS/CMSIS/Core/Include"
"Middlewares/CMSIS/CMSIS/Core/Include/m-profile"
"Middlewares/CherryUSB/class/cdc"
"Middlewares/CherryUSB/common"
"Middlewares/CherryUSB/core"
)

set(DRIVER_INCLUDE_DIRS
"driver/include/"
)

set(USER_INCLUDE_DIRS
"src/config/include"
"src/prot"
"src"
)

set(USER_SOURCES
"src/main.c"
"src/prot/usb_dc.c"
)

set(USER_DEFINITIONS
    "__PLACEHOLDER_DEFINE_1__"
    "__PLACEHOLDER_DEFINE_2__"
)

set(USER_COMPILE_OPTIONS
    "__PLACEHOLDER_COMPILE_OPTION_1__"
    "__PLACEHOLDER_COMPILE_OPTION_2__"
)

set(USER_LINK_OPTIONS
    "-Wl,--gc-sections"
    "-Wl,--print-memory-usage"
    "-Wl,-Map=${CMAKE_BINARY_DIR}/${FIRMWARE_TARGET}.map"
    "-specs=nosys.specs"
)

set(USER_LINK_LIBRARIES
    "__PLACEHOLDER_LINK_LIBRARY_1__"
)

set(USER_LINKER_SCRIPT "driver/linkscript/apm32f402_403xb_flash.ld")

# Filter out placeholders
list(FILTER USER_SOURCES EXCLUDE REGEX "${PLACEHOLDER_PREFIX}")
list(FILTER USER_INCLUDE_DIRS EXCLUDE REGEX "${PLACEHOLDER_PREFIX}")
list(FILTER USER_DEFINITIONS EXCLUDE REGEX "${PLACEHOLDER_PREFIX}")
list(FILTER USER_COMPILE_OPTIONS EXCLUDE REGEX "${PLACEHOLDER_PREFIX}")
list(FILTER USER_LINK_OPTIONS EXCLUDE REGEX "${PLACEHOLDER_PREFIX}")
list(FILTER USER_LINK_LIBRARIES EXCLUDE REGEX "${PLACEHOLDER_PREFIX}")

add_executable(${FIRMWARE_TARGET})

set(CMAKE_PLACEHOLDER_SOURCE "${CMAKE_BINARY_DIR}/cmake_placeholder.c")
file(WRITE "${CMAKE_PLACEHOLDER_SOURCE}" "void __cmake_placeholder(void) {}\n")
target_sources(${FIRMWARE_TARGET} PRIVATE "${CMAKE_PLACEHOLDER_SOURCE}")

if(USER_SOURCES)
    target_sources(${FIRMWARE_TARGET} PRIVATE ${USER_SOURCES})
endif()

if(MIDDLEWARES_INCLUDE_DIRS)
    target_include_directories(${FIRMWARE_TARGET} PRIVATE ${MIDDLEWARES_INCLUDE_DIRS})
endif()

if(DRIVER_INCLUDE_DIRS)
    target_include_directories(${FIRMWARE_TARGET} PRIVATE ${DRIVER_INCLUDE_DIRS})
endif()

if(USER_INCLUDE_DIRS)
    target_include_directories(${FIRMWARE_TARGET} PRIVATE ${USER_INCLUDE_DIRS})
endif()

if(USER_DEFINITIONS)
    target_compile_definitions(${FIRMWARE_TARGET} PRIVATE ${USER_DEFINITIONS})
endif()

if(USER_COMPILE_OPTIONS)
    target_compile_options(${FIRMWARE_TARGET} PRIVATE ${USER_COMPILE_OPTIONS})
endif()

if(USER_LINK_OPTIONS)
    target_link_options(${FIRMWARE_TARGET} PRIVATE ${USER_LINK_OPTIONS})
endif()

if(USER_LINK_LIBRARIES)
    target_link_libraries(${FIRMWARE_TARGET} PRIVATE ${USER_LINK_LIBRARIES})
endif()

if(NOT USER_LINKER_SCRIPT MATCHES "${PLACEHOLDER_PREFIX}")
    target_link_options(${FIRMWARE_TARGET} PRIVATE "-T${USER_LINKER_SCRIPT}")
endif()
