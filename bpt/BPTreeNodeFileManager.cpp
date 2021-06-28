//
// Created by smilu97 on 21. 6. 27..
//

#include "BPTreeNodeFileManager.h"

BPTreeNodeFileManager::BPTreeNodeFileManager(
    const std::string &filepath,
    std::unique_ptr<CachePolicy> policy,
    uint32_t cacheSize): _buf(filepath, std::move(policy), cacheSize) {

}

uint8_t * BPTreeNodeFileManager::Read(uint32_t page, uint32_t offset, uint32_t size) {
    return _buf.GetPage(page) + offset;
}

void BPTreeNodeFileManager::Write(uint8_t *buf, uint32_t page, uint32_t offset, uint32_t size) {
    uint8_t * dest = _buf.GetPage(page) + offset;
    memcpy(dest, buf, size);
}

void BPTreeNodeFileManager::Write8(uint8_t value, uint32_t page, uint32_t offset) {
    uint8_t * buf = &value;
    Write(buf, page, offset, 1);
}

void BPTreeNodeFileManager::Write16(uint16_t value, uint32_t page, uint32_t offset) {
    uint8_t * buf = (uint8_t*) &value;
    Write(buf, page, offset, 2);
}

void BPTreeNodeFileManager::Write32(uint32_t value, uint32_t page, uint32_t offset) {
    uint8_t * buf = (uint8_t*) &value;
    Write(buf, page, offset, 4);
}

void BPTreeNodeFileManager::Write64(uint64_t value, uint32_t page, uint32_t offset) {
    uint8_t * buf = (uint8_t*) &value;
    Write(buf, page, offset, 8);
}
