#include "FileManager.h"

FileManager::FileManager(const std::string &filepath) {
    const int flags = O_CREAT | O_DIRECT | O_SYNC;
    const mode_t mode = 0x664;
    _fd = open(filepath.c_str(), flags, mode);
    if (_fd < 0) {
        _HandleFileOpenError();
    }
}

FileManager::~FileManager() {
    close(_fd);
}

void FileManager::Read(void *buf, uint offset, uint size) {
    lseek(_fd, offset, SEEK_SET);
    read(_fd, buf, size);
}

void FileManager::Write(void *buf, uint offset, uint size) {
    lseek(_fd, offset, SEEK_SET);
    write(_fd, buf, size);
}

void FileManager::_HandleFileOpenError() {
    if (errno == EACCES) {

    }
}
