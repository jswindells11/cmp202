cmake_minimum_required(VERSION 3.25)
project(Mandelbrot)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(cpuinfo)

if(WIN32)
    add_link_options(/STACK:10000000)
else()
    set(CMAKE_EXE_LINKER_FLAGS "-Wl,-z,stack-size=1000000000 ${CMAKE_EXE_LINKER_FLAGS} -pthread")
endif()

add_executable(Mandelbrot
        main.cpp
        mandelbrot.cpp
        mandelbrot.h
        tga.cpp
        tga.h log_thread.cpp log_thread.h)
target_link_libraries(Mandelbrot cpuinfo)