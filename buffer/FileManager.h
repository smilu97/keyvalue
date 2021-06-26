//
// Created by smilu97 on 21. 6. 27..
//

#ifndef KEYVALUE_FILEMANAGER_H
#define KEYVALUE_FILEMANAGER_H

#include <string>
#include <unistd.h>
#include <fcntl.h>

class FileManager {
    int _fd;
public:
    FileManager(const std::string & filepath);
    ~FileManager();
    void Read(void* buf, uint offset, uint size);
    void Write(void* buf, uint offset, uint size);
private:
    void _HandleFileOpenError();
};

#endif //KEYVALUE_FILEMANAGER_H
