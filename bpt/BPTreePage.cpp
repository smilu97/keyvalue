//
// Created by smilu97 on 21. 6. 28..
//

#include "BPTreePage.h"

template<class PageStruct>
BPTreePage<PageStruct>::BPTreePage(const std::shared_ptr<BPTreeNodeManager> &nodeMan): _nodeMan(nodeMan) {

}

template<class PageStruct>
void BPTreePage<PageStruct>::Update(uint32_t offset, uint8_t *buf, uint32_t size) {
    auto nodeMan = _nodeMan.lock();
    nodeMan->Write(buf, 0, offset, size);
}

template<class PageStruct>
void BPTreePage<PageStruct>::Update(uint32_t offset, uint8_t value) {
    auto *buf = (uint8_t*) &value;
    Update(offset, buf, sizeof(value));
}

template<class PageStruct>
void BPTreePage<PageStruct>::Update(uint32_t offset, uint16_t value) {
    auto *buf = (uint8_t*) &value;
    Update(offset, buf, sizeof(value));
}

template<class PageStruct>
void BPTreePage<PageStruct>::Update(uint32_t offset, uint32_t value) {
    auto *buf = (uint8_t*) &value;
    Update(offset, buf, sizeof(value));
}

template<class PageStruct>
void BPTreePage<PageStruct>::Update(uint32_t offset, uint64_t value) {
    auto *buf = (uint8_t*) &value;
    Update(offset, buf, sizeof(value));
}
