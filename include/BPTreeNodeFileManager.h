//
// Created by smilu97 on 21. 6. 27..
//

#ifndef KEYVALUE_BPTREENODEFILEMANAGER_H
#define KEYVALUE_BPTREENODEFILEMANAGER_H

#include "BPTreeNodeManager.h"
#include "FileBuffer.h"

#include <cstring>

class BPTreeNodeFileManager: public BPTreeNodeManager {
    FileBuffer _buf;
public:
    BPTreeNodeFileManager(const std::string &filepath,
                          std::unique_ptr<CachePolicy> policy,
                          uint32_t cacheSize);
    byte* Read(page_t page, offset_t offset, size_t size) override;
    void Write(const byte *buf, page_t page, offset_t offset, size_t size) override;
    void Write8(uint8_t value, page_t page, offset_t offset) override;
    void Write16(uint16_t value, page_t page, offset_t offset) override;
    void Write32(uint32_t value, page_t page, offset_t offset) override;
    void Write64(uint64_t value, page_t page, offset_t offset) override;
};


#endif //KEYVALUE_BPTREENODEFILEMANAGER_H
