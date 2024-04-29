# ----------------------------------------------------------------------------
# Copyright (c) 2024 Thomas Buck (thomas@xythobuz.de)
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# See <http://www.gnu.org/licenses/>.
# ----------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.13)

# initialize pico-sdk from submodule
include(pico-sdk/pico_sdk_init.cmake)

project(gadget C CXX)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

add_executable(drumkit)

target_sources(drumkit PUBLIC
    src/adc.c
    src/buttons.c
    src/encoder.c
    src/lcd.c
    src/led.c
    src/logo.c
    src/main.c
    src/pulse.c
    src/sequence.c
    src/ui.c
    src/log.c
    src/ring.c
    src/console.c
    src/usb.c
    src/usb_cdc.c
    src/usb_descriptors.c
    src/usb_midi.c
    src/settings.c
    src/mem.c

    pico-ssd1306/ssd1306.c
)

# external dependency include directories
target_include_directories(drumkit PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
target_include_directories(drumkit PUBLIC ${CMAKE_CURRENT_LIST_DIR}/pico-ssd1306)

# enable generous warnings
target_compile_options(drumkit PUBLIC
    -Wall
    -Wextra
    -Werror
    -Wshadow
    -O3

    -DSSD1306_DEBUG_PRINT=debug
    -DSSD1306_DEBUG_INCLUDE="log.h"
)

# suppress some warnings for borked 3rd party files in Pico SDK
set_source_files_properties(pico-sdk/lib/btstack/src/ble/sm.c PROPERTIES COMPILE_FLAGS -Wno-unused-parameter)
set_source_files_properties(pico-sdk/lib/btstack/src/btstack_hid_parser.c PROPERTIES COMPILE_FLAGS -Wno-maybe-uninitialized)
set_source_files_properties(pico-sdk/src/rp2_common/pico_cyw43_driver/cyw43_driver.c PROPERTIES COMPILE_FLAGS -Wno-unused-parameter)
set_source_files_properties(pico-sdk/lib/btstack/src/classic/avdtp_util.c PROPERTIES COMPILE_FLAGS -Wno-unused-parameter)
set_source_files_properties(pico-sdk/lib/btstack/src/classic/goep_client.c PROPERTIES COMPILE_FLAGS -Wno-unused-parameter)
set_source_files_properties(pico-sdk/lib/btstack/src/classic/goep_server.c PROPERTIES COMPILE_FLAGS -Wno-unused-parameter)
set_source_files_properties(pico-sdk/src/rp2_common/hardware_flash/flash.c PROPERTIES COMPILE_FLAGS -Wno-shadow)

# repo meta data
include(FetchContent)
FetchContent_Declare(cmake_git_version_tracking
    GIT_REPOSITORY https://github.com/andrew-hardin/cmake-git-version-tracking.git
    GIT_TAG 6c0cb87edd029ddfb403a8e24577c144a03605a6
)
FetchContent_MakeAvailable(cmake_git_version_tracking)

# pull in common dependencies
target_link_libraries(drumkit
    pico_stdlib
    hardware_i2c
    hardware_adc
    hardware_gpio
    hardware_pwm
    pico_unique_id
    tinyusb_device
    tinyusb_board
    cmake_git_version_tracking
    hardware_flash
    pico_flash
    hardware_exception
)

pico_set_linker_script(drumkit ${CMAKE_CURRENT_LIST_DIR}/src/memmap_custom.ld)

# fix for Errata RP2040-E5 (the fix requires use of GPIO 15)
target_compile_definitions(drumkit PUBLIC PICO_RP2040_USB_DEVICE_ENUMERATION_FIX=1)

pico_add_extra_outputs(drumkit)

# print binary size
# https://github.com/raspberrypi/pico-sdk/issues/1196#issuecomment-1429404558
add_custom_command(TARGET drumkit
    POST_BUILD
    COMMAND ${PICO_GCC_TRIPLE}-size --format=gnu ${CMAKE_CURRENT_BINARY_DIR}/drumkit.elf
)
