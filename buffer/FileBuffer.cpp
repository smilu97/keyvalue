//
// Created by smilu97 on 21. 6. 27..
//

#include "FileBuffer.h"

FileBuffer::FileBuffer(
        const std::string &filepath,
        std::unique_ptr<CachePolicy> policy,
        size_t sz): _file(filepath), _sz(sz) {
    _buf =  new byte[PAGE_SIZE * sz];
    _policy = std::move(policy);
}

FileBuffer::~FileBuffer() {
    delete[] _buf;
}

void FileBuffer::flush() const {
    for (uint64_t i = 0; i < _policy->Size(); i++) {
        std::optional<uint64_t> op = _policy->GetRoom(i);
        if (!op.has_value()) continue;
        uint64_t p = op.value();
        uint8_t * buf = _buf + (PAGE_SIZE * i);
        _file.Write(buf, PAGE_SIZE * p, PAGE_SIZE);
        std::cout << "Flushed page " << p << std::endl;
    }
}

uint8_t* FileBuffer::GetPage(page_t n) {
    const auto index = _policy->Get(n);
    if (index.has_value()) {
        return _buf + (PAGE_SIZE * index.value());
    }
    return LoadPage(n);
}

uint8_t* FileBuffer::LoadPage(page_t n) {
    auto replace = _policy->Load(n);
    uint8_t * buf = _buf + (PAGE_SIZE * replace.first);
    if (replace.second.has_value()) {
        _file.Write(buf, PAGE_SIZE * replace.second.value(), PAGE_SIZE);
        std::cout << "Flushed page " << replace.second.value() << " for replace" << std::endl;
    }
    _file.Read(buf, PAGE_SIZE * n, PAGE_SIZE);
    return buf;
}
