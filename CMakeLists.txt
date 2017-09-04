cmake_minimum_required(VERSION 3.8)
project(C_Servers)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        Sources/list.c
        Headers/list.h
        Sources/main.c
        Sources/util.c
        Headers/util.h)

add_executable(C_Servers ${SOURCE_FILES})