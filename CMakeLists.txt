cmake_minimum_required(VERSION 3.5)
project(pttask)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra")

set(SOURCE_FILES
    IDataHandler.h
    IStreamReader.h
    main.cpp
    StreamReader.cpp
    StreamReader.h)

add_executable(pttask ${SOURCE_FILES})
