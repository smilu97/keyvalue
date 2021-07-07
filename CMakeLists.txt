cmake_minimum_required(VERSION 3.19)
project(keyvalue)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_UNITY_BUILD TRUE)
set(CMAKE_UNITY_BUILD_BATCH_SIZE 99999)

include_directories(./include)

add_executable(keyvalue
        main.cpp
        util.cpp
        buffer/FileManager.cpp
        buffer/FileBuffer.cpp
        buffer/SecondChance.cpp
        bpt/BPTree.cpp
        bpt/BPTreeLeafNode.cpp
        bpt/BPTreeInternalNode.cpp
        bpt/BPTreeNodeFileManager.cpp
        bpt/BPTreeHeader.cpp
        bpt/BPTreePage.cpp
        bpt/BPTreeFreePage.cpp)