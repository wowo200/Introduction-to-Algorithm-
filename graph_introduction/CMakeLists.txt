cmake_minimum_required(VERSION 2.6)
project(graph_introduction)
add_compile_options(-std=c++11)
add_executable(graph_introduction main.cpp)

install(TARGETS graph_introduction RUNTIME DESTINATION bin)
