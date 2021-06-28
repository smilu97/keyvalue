cmake_minimum_required(VERSION 3.19)
project(keyvalue)

set(CMAKE_CXX_STANDARD 17)

include_directories(./)

add_executable(keyvalue
        main.cpp
        util.cpp util.h
        buffer/FileManager.cpp buffer/FileManager.h
        buffer/FileBuffer.cpp buffer/FileBuffer.h
        buffer/SecondChance.cpp buffer/SecondChance.h
        buffer/CachePolicy.h
        bpt/BPTree.cpp bpt/BPTree.h
        bpt/BPTreeLeafNode.cpp bpt/BPTreeLeafNode.h
        bpt/BPTreeInternalNode.cpp bpt/BPTreeInternalNode.h
        bpt/BPTreeNodeManager.h
        bpt/BPTreeNodeFileManager.cpp bpt/BPTreeNodeFileManager.h bpt/BPTreeHeader.cpp bpt/BPTreeHeader.h common.h bpt/BPTreePage.cpp bpt/BPTreePage.h)