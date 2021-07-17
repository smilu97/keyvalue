#include "FileManager.h"

#include <iostream>
#include <unistd.h>
#include <fcntl.h>

FileManager::FileManager(const std::string &filepath) {
    const int flags = O_RDWR | O_CREAT | O_SYNC;
    const mode_t mode = 0644;
    _fd = open(filepath.c_str(), flags, mode);
    if (_fd < 0) {
        HandleFileOpenError();
    }
}

FileManager::~FileManager() {
    close(_fd);
}

void FileManager::Read(void *buf, uint offset, uint size) const {
    if (pread(_fd, buf, size, offset) < 0)
        panic("pread error");
}

void FileManager::Write(void *buf, uint offset, uint size) const {
    if (pwrite(_fd, buf, size, offset) < 0) {
        std::cerr << "errno: " << errno << std::endl;
        panic("pwrite error");
    }
}

void FileManager::HandleFileOpenError() {
    if (errno == EACCES) {
        std::cerr << "Permission denied" << std::endl;
    } else {
        std::cerr << "Unknown error: Cannot open file" << std::endl;
    }
}
