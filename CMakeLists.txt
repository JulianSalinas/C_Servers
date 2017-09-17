cmake_minimum_required(VERSION 3.8)
project(C_Servers)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES Sources/main.c Sources/Util/list.c Headers/Util/list.h Sources/Util/util.c Headers/Util/util.h Headers/main.h Sources/client.c Headers/client.h Sources/server_t.c Headers/server_t.h Sources/server_f.c Headers/server_f.h Headers/server.h Sources/server_k.c Headers/server_k.h Sources/server_p.c Headers/server_p.h Sources/server.c Sources/http.c Headers/http.h)
find_package( Threads REQUIRED )
add_executable(C_Servers ${SOURCE_FILES})
target_link_libraries( C_Servers Threads::Threads )