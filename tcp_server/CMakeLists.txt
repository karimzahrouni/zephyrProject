# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)
set(CMAKE_C_STANDARD 11)

add_compile_options(-std=c11)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(network)

target_include_directories(app PRIVATE inc)
target_sources(app PRIVATE src/main.c src/network.c)
