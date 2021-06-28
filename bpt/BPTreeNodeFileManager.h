//
// Created by smilu97 on 21. 6. 27..
//

#ifndef KEYVALUE_BPTREENODEFILEMANAGER_H
#define KEYVALUE_BPTREENODEFILEMANAGER_H

#include "BPTreeNodeManager.h"
#include "buffer/FileBuffer.h"

#include <cstring>

class BPTreeNodeFileManager: public BPTreeNodeManager {
    FileBuffer _buf;
public:
    BPTreeNodeFileManager(const std::string &filepath,
                          std::unique_ptr<CachePolicy> policy,
                          uint32_t cacheSize);
    uint8_t* Read(uint32_t page, uint32_t offset, uint32_t size) override;
    void Write(uint8_t *buf, uint32_t page, uint32_t offset, uint32_t size) override;
    void Write8(uint8_t value, uint32_t page, uint32_t offset) override;
    void Write16(uint16_t value, uint32_t page, uint32_t offset) override;
    void Write32(uint32_t value, uint32_t page, uint32_t offset) override;
    void Write64(uint64_t value, uint32_t page, uint32_t offset) override;
};


#endif //KEYVALUE_BPTREENODEFILEMANAGER_H
