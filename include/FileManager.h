//
// Created by smilu97 on 21. 6. 27..
//

#ifndef KEYVALUE_FILEMANAGER_H
#define KEYVALUE_FILEMANAGER_H

#include <iostream>
#include <string>
#include <unistd.h>
#include <fcntl.h>

class FileManager {
    int _fd;
public:
    explicit FileManager(const std::string & filepath);
    ~FileManager();
    void Read(void* buf, uint offset, uint size) const;
    void Write(void* buf, uint offset, uint size) const;
private:
    static void HandleFileOpenError();
};

#endif //KEYVALUE_FILEMANAGER_H
