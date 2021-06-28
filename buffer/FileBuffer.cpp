//
// Created by smilu97 on 21. 6. 27..
//

#include "FileBuffer.h"

FileBuffer::FileBuffer(
        const std::string &filepath,
        std::unique_ptr<CachePolicy> policy,
        uint32_t sz): _file(filepath), _sz(sz) {
    _buf = (uint8_t*) malloc(PAGE_SIZE * sz);
    _policy = std::move(policy);
}

uint8_t* FileBuffer::GetPage(uint32_t n) {
    const auto index = _policy->Get(n);
    if (index.has_value()) {
        return _buf + (PAGE_SIZE * index.value());
    }
    return LoadPage(n);
}

uint8_t* FileBuffer::LoadPage(uint32_t n) {
    auto replace = _policy->Load(n);
    uint8_t * buf = _buf + (PAGE_SIZE * replace.first);
    if (replace.second.has_value()) {
        _file.Write(buf, PAGE_SIZE * replace.second.value(), PAGE_SIZE);
    }
    _file.Read(buf, PAGE_SIZE * n, PAGE_SIZE);
    return buf;
}
