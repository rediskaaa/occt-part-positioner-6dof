cmake_minimum_required(VERSION 3.15)
project(tsan_demo CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Опция для включения Thread Sanitizer
option(ENABLE_TSAN "Enable Thread Sanitizer" OFF)

add_executable(main main.cpp)

if(ENABLE_TSAN)
    message(STATUS "Thread Sanitizer ENABLED")
    # Флаги компиляции и линковки для TSan
    target_compile_options(main PRIVATE -fsanitize=thread -g -O1 -fno-omit-frame-pointer)
    target_link_options(main PRIVATE -fsanitize=thread)
else()
    message(STATUS "Thread Sanitizer DISABLED")
endif()