cmake_minimum_required(VERSION 3.20)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

if(NOT CMAKE_C_COMPILER)
    set(CMAKE_C_COMPILER "C:/Program Files/ArmTools/gcc-arm-none-eabi-10.3-2021.10/bin/arm-none-eabi-gcc.exe")
endif()

if(NOT CMAKE_ASM_COMPILER)
    set(CMAKE_ASM_COMPILER "C:/Program Files/ArmTools/gcc-arm-none-eabi-10.3-2021.10/bin/arm-none-eabi-gcc.exe")
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
"src/port"
"src"
)

set(USER_SOURCES
"src/main.c"
"src/port/usb_dc.c"
"src/config/source/delay.c"
"src/config/source/device_cfg.c"
"src/config/source/usb_cfg.c"
"Middlewares/CherryUSB/core/usbd_core.c"
"Middlewares/CherryUSB/class/cdc/usbd_cdc_acm.c"
)

set(DRIVER_SOURCES
"driver/src/apm32f402_403_gpio.c"
"driver/src/apm32f402_403_misc.c"
"driver/src/apm32f402_403_rcm.c"
"driver/src/apm32f402_403_tmr.c"
"driver/src/apm32f402_403_usb.c"
"driver/src/system_apm32f402_403.c"
)

set(USER_DEFINITIONS
    "APM32F402xx"
)

set(USER_COMPILE_OPTIONS
    "-mcpu=cortex-m4"
    "-mthumb"
    "-mfloat-abi=hard"
    "-g"
    "-O0"
)

set(USER_LINK_OPTIONS
    "-mcpu=cortex-m4"
    "-mthumb"
    "-mfloat-abi=hard"
    "-mfpu=fpv4-sp-d16"
    "-Wl,--gc-sections"
    "-Wl,--print-memory-usage"
    "-Wl,-Map=${CMAKE_BINARY_DIR}/${FIRMWARE_TARGET}.map"
    "--specs=nosys.specs"
)

set(USER_LINK_LIBRARIES
    "__PLACEHOLDER_LINK_LIBRARY_1__"
)

set(USER_LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/driver/linkscript/apm32f402_403xb_flash.ld")

set(USER_STARTUP_FILE "${CMAKE_SOURCE_DIR}/driver/start/startup_apm32f402_403.S")

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

if(DRIVER_SOURCES)
    target_sources(${FIRMWARE_TARGET} PRIVATE ${DRIVER_SOURCES})
endif()

if(USER_STARTUP_FILE)
    target_sources(${FIRMWARE_TARGET} PRIVATE ${USER_STARTUP_FILE})
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
