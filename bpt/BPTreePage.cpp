//
// Created by smilu97 on 21. 6. 28..
//

#include "BPTreePage.h"

template<class PageStruct>
BPTreePage<PageStruct>::BPTreePage(const std::shared_ptr<BPTreeNodeManager> &nodeMan, uint32_t page):
    _nodeMan(nodeMan), _page(page) {

}

template<class PageStruct>
void BPTreePage<PageStruct>::Update(uint32_t offset, uint8_t *buf, uint32_t size) {
    _nodeMan.lock()->Write(buf, _page, offset, size);
}

template<class PageStruct>
void BPTreePage<PageStruct>::Update(uint32_t offset, uint8_t value) {
    _nodeMan.lock()->Write8(value, _page, offset);
}

template<class PageStruct>
void BPTreePage<PageStruct>::Update(uint32_t offset, uint16_t value) {
    _nodeMan.lock()->Write16(value, _page, offset);
}

template<class PageStruct>
void BPTreePage<PageStruct>::Update(uint32_t offset, uint32_t value) {
    _nodeMan.lock()->Write32(value, _page, offset);
}

template<class PageStruct>
void BPTreePage<PageStruct>::Update(uint32_t offset, uint64_t value) {
    _nodeMan.lock()->Write64(value, _page, offset);
}

template<class PageStruct>
uint32_t BPTreePage<PageStruct>::GetType() {
    /**
     * The first 4-bytes of every page must be uint32-type descriptor
     */
    return *((uint32_t*) Header());
}

template<class PageStruct>
void BPTreePage<PageStruct>::SetType(uint32_t _type) {
    Update(0, _type);
}

template<class PageStruct>
uint8_t *BPTreePage<PageStruct>::Read(uint32_t offset, uint32_t size) const {
    auto nodeMan = _nodeMan.lock();
    return nodeMan->Read(_page, offset, size);
}

template<class PageStruct>
uint8_t BPTreePage<PageStruct>::Read8(uint32_t offset) const {
    return *((uint8_t*) Read(offset, sizeof(uint8_t)));
}

template<class PageStruct>
uint16_t BPTreePage<PageStruct>::Read16(uint32_t offset) const {
    return *((uint16_t*) Read(offset, sizeof(uint16_t)));
}

template<class PageStruct>
uint32_t BPTreePage<PageStruct>::Read32(uint32_t offset) const {
    return *((uint32_t*) Read(offset, sizeof(uint32_t)));
}

template<class PageStruct>
uint64_t BPTreePage<PageStruct>::Read64(uint32_t offset) const {
    return *((uint64_t*) Read(offset, sizeof(uint64_t)));
}

template<class PageStruct>
bool BPTreePage<PageStruct>::IsHeader() {
    return GetType() == BPTREE_PAGE_TYPE_HEADER;
}

template<class PageStruct>
bool BPTreePage<PageStruct>::IsInternal() {
    return GetType() == BPTREE_PAGE_TYPE_INTERNAL;
}

template<class PageStruct>
bool BPTreePage<PageStruct>::IsLeaf() {
    return GetType() == BPTREE_PAGE_TYPE_LEAF;
}

template<class PageStruct>
const PageStruct *BPTreePage<PageStruct>::Header() {
    auto nodeMan = _nodeMan.lock();
    return (PageStruct*) nodeMan->Read(_page, 0, BPTREE_HEADER_SIZE);
}
