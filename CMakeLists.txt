cmake_minimum_required(VERSION 3.22)

project(interview_tasks
        DESCRIPTION "Interview tasks"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED 17)
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

add_executable(task1)

target_sources(task1
        PRIVATE
        main.cpp)