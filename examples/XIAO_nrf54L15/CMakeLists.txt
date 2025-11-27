#
# Copyright (c) 2018 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#
cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(NONE)

# NORDIC SDK APP START
target_sources(app PRIVATE src/main.cpp)
target_sources(app PRIVATE src/adc.c)
target_sources(app PRIVATE src/bme280.c)
target_sources(app PRIVATE src/wdt.c)
target_sources(app PRIVATE ../../src/BaseDevice.cpp)
target_sources(app PRIVATE ../../src/BtHomeV2Device.cpp)

# NORDIC SDK APP END
zephyr_library_include_directories(.)

