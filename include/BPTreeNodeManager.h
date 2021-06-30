//
// Created by smilu97 on 21. 6. 27..
//

#ifndef KEYVALUE_BPTREENODEMANAGER_H
#define KEYVALUE_BPTREENODEMANAGER_H

#include <cstdint>

#include "common.h"

class BPTreeNodeManager {
public:
    virtual byte* Read(page_t page, offset_t offset, size_t size) = 0;
    virtual void Write(const byte *buf, page_t page, offset_t offset, size_t size) = 0;
    virtual void Write8(uint8_t value, page_t page, offset_t offset) = 0;
    virtual void Write16(uint16_t value, page_t page, offset_t offset) = 0;
    virtual void Write32(uint32_t value, page_t page, offset_t offset) = 0;
    virtual void Write64(uint64_t value, page_t page, offset_t offset) = 0;
};


#endif //KEYVALUE_BPTREENODEMANAGER_H
