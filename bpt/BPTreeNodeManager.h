//
// Created by smilu97 on 21. 6. 27..
//

#ifndef KEYVALUE_BPTREENODEMANAGER_H
#define KEYVALUE_BPTREENODEMANAGER_H

#include <cstdint>

class BPTreeNodeManager {
public:
    virtual uint8_t* Read(uint32_t page, uint32_t offset, uint32_t size) = 0;
    virtual void Write(uint8_t *buf, uint32_t page, uint32_t offset, uint32_t size) = 0;
    virtual void Write8(uint8_t value, uint32_t page, uint32_t offset) = 0;
    virtual void Write16(uint16_t value, uint32_t page, uint32_t offset) = 0;
    virtual void Write32(uint32_t value, uint32_t page, uint32_t offset) = 0;
    virtual void Write64(uint64_t value, uint32_t page, uint32_t offset) = 0;
};


#endif //KEYVALUE_BPTREENODEMANAGER_H
