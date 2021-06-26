//
// Created by smilu97 on 21. 6. 27..
//

#include "FileBuffer.h"

FileBuffer::FileBuffer(const std::string &filepath, uint sz): _file(filepath), _sz(sz) {
    _buf = (uint8_t*) malloc(PAGE_SIZE * sz);
    _policy = std::auto_ptr<CachePolicy>(new SecondChance(sz));
}

void* FileBuffer::GetPage(int index) {
    auto victim = _policy->push(index);
    uint8_t * buf = _buf + (PAGE_SIZE * victim.first);
    _file.Write(buf, PAGE_SIZE * victim.second, PAGE_SIZE);
    _file.Read(buf, PAGE_SIZE * index, PAGE_SIZE);
    return buf;
}
