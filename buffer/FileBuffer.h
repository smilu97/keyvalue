//
// Created by smilu97 on 21. 6. 27..
//

#ifndef KEYVALUE_FILEBUFFER_H
#define KEYVALUE_FILEBUFFER_H

#include "FileManager.h"
#include "SecondChance.h"

#include <memory>

#define PAGE_SIZE (0x1000)

class FileBuffer {
    FileManager _file;
    uint _sz;
    uint8_t * _buf;
    std::auto_ptr<CachePolicy> _policy;
public:
    FileBuffer(const std::string& filepath, uint sz);
    void* GetPage(int index);
};


#endif //KEYVALUE_FILEBUFFER_H
