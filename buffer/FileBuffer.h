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
    uint8_t * _buf;
    std::unique_ptr<CachePolicy> _policy;
public:
    FileBuffer(const std::string &filepath, std::unique_ptr<CachePolicy> policy, uint32_t sz);
    uint8_t* GetPage(uint32_t n);
private:
    uint8_t* LoadPage(uint32_t n);
};


#endif //KEYVALUE_FILEBUFFER_H
