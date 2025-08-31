cmake_minimum_required(VERSION 3.28)
project(effective_mobile)

set(CMAKE_CXX_STANDARD 20)

add_executable(test_task
        test/main.cpp
        test/Bitmap.cpp
        test/Bitmap.h
)

add_subdirectory(second)
