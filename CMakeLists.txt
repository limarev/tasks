cmake_minimum_required(VERSION 3.22)

project(interview_tasks
        DESCRIPTION "Interview tasks"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED 17)
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

add_executable(task1)

target_sources(task1
        PRIVATE
        task1.cpp
        util.h)

target_compile_options(task1 PRIVATE -fsanitize=thread)
target_link_options(task1 PRIVATE -fsanitize=thread)
