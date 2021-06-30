//
// Created by smilu97 on 21. 6. 27..
//

#ifndef KEYVALUE_COMMON_H
#define KEYVALUE_COMMON_H

#include <cstdint>

#define PAGE_SIZE (0x1000)

#define BPTREE_HEADER_SIZE (128)

enum BPTreePageType {
    BPTREE_PAGE_TYPE_HEADER = 0,
    BPTREE_PAGE_TYPE_INTERNAL,
    BPTREE_PAGE_TYPE_LEAF,
    BPTREE_PAGE_TYPE_FREE
};

typedef uint64_t page_t;
typedef uint64_t offset_t;
typedef uint8_t byte;
typedef uint64_t size_t;

#endif //KEYVALUE_COMMON_H
