//
// Created by smilu97 on 21. 6. 27..
//

#include "BPTreeNodeFileManager.h"

BPTreeNodeFileManager::BPTreeNodeFileManager(
    const std::string &filepath,
    std::unique_ptr<CachePolicy> policy,
    uint32_t cacheSize): _buf(filepath, std::move(policy), cacheSize) {

}

byte * BPTreeNodeFileManager::Read(page_t page, offset_t offset, size_t size) {
    return _buf.GetPage(page) + offset;
}

void BPTreeNodeFileManager::Write(const byte *buf, page_t page, offset_t offset, size_t size) {
    uint8_t * dest = _buf.GetPage(page) + offset;
    memcpy(dest, buf, size);
}

void BPTreeNodeFileManager::Write8(uint8_t value, page_t page, offset_t offset) {
    uint8_t * dest = ((uint8_t*) (_buf.GetPage(page) + offset));
    *dest = value;
}

void BPTreeNodeFileManager::Write16(uint16_t value, page_t page, offset_t offset) {
    uint16_t * dest = ((uint16_t*) (_buf.GetPage(page) + offset));
    *dest = value;
}

void BPTreeNodeFileManager::Write32(uint32_t value, page_t page, offset_t offset) {
    uint32_t * dest = ((uint32_t*) (_buf.GetPage(page) + offset));
    *dest = value;
}

void BPTreeNodeFileManager::Write64(uint64_t value, page_t page, offset_t offset) {
    uint64_t * dest = ((uint64_t*) (_buf.GetPage(page) + offset));
    *dest = value;
}
