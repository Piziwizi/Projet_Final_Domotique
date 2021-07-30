cmake_minimum_required(VERSION 3.10)
enable_language(C)
enable_language(CXX)

set(TARGET backend)

project(${TARGET})

add_executable(${TARGET} 
                src/main.c 
                src/sensor.c
                src/logging.c 
                src/interface.c)

target_link_libraries (backend PUBLIC pthread)
target_link_libraries (backend PUBLIC json-c)
