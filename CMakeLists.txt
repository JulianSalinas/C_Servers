cmake_minimum_required(VERSION 3.8)
project(C_Servers)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES Sources/main.c Sources/list.c Headers/list.h Sources/util.c Headers/util.h Headers/main.h Sources/client.c Headers/client.h Sources/server.c Headers/server.h Sources/tests.c Headers/tests.h)
find_package( Threads REQUIRED )
add_executable(C_Servers ${SOURCE_FILES})
target_link_libraries( C_Servers Threads::Threads )