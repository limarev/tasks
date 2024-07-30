cmake_minimum_required(VERSION 3.22)

project(template
        VERSION 1.0.0
        DESCRIPTION "How template works"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED 23)
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

add_executable(template)

target_sources(template
        PRIVATE
        main.cpp)