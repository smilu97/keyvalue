#include "FileManager.h"

FileManager::FileManager(const std::string &filepath) {
    const int flags = O_CREAT | O_DIRECT | O_SYNC;
    const mode_t mode = 0x664;
    _fd = open(filepath.c_str(), flags, mode);
    if (_fd < 0) {
        HandleFileOpenError();
    }
}

FileManager::~FileManager() {
    close(_fd);
}

void FileManager::Read(void *buf, uint offset, uint size) const {
    lseek(_fd, offset, SEEK_SET);
    read(_fd, buf, size);
}

void FileManager::Write(void *buf, uint offset, uint size) const {
    lseek(_fd, offset, SEEK_SET);
    write(_fd, buf, size);
}

void FileManager::HandleFileOpenError() {
    if (errno == EACCES) {
        std::cerr << "Permission denied" << std::endl;
    } else {
        std::cerr << "Unknown error: Cannot open file" << std::endl;
    }
}
