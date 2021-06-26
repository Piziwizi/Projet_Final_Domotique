cmake_minimum_required(VERSION 3.10)

# set the project name
project(backend)

# add the executable
add_executable(backend main.c)

target_link_libraries (backend PUBLIC pthread)