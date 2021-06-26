cmake_minimum_required(VERSION 3.19)
project(keyvalue)

set(CMAKE_CXX_STANDARD 14)

add_executable(keyvalue
        main.cpp
        buffer/FileManager.cpp
        util.cpp util.h
        buffer/FileManager.h buffer/FileBuffer.cpp
        buffer/FileBuffer.h buffer/SecondChance.cpp
        buffer/SecondChance.h buffer/CachePolicy.h
        )