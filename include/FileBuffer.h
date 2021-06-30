//
// Created by smilu97 on 21. 6. 27..
//

#ifndef KEYVALUE_FILEBUFFER_H
#define KEYVALUE_FILEBUFFER_H

#include "FileManager.h"
#include "SecondChance.h"
#include "common.h"

#include <memory>
#include <unordered_map>

class FileBuffer {
    FileManager _file;
    uint _sz;
    byte * _buf;
    std::unique_ptr<CachePolicy> _policy;
public:
    FileBuffer(const std::string &filepath, std::unique_ptr<CachePolicy> policy, size_t sz);
    ~FileBuffer();
    byte* GetPage(page_t n);
private:
    byte* LoadPage(page_t n);
};


#endif //KEYVALUE_FILEBUFFER_H
