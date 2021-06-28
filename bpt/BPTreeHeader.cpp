//
// Created by smilu97 on 21. 6. 27..
//

#include "BPTreeHeader.h"

BPTreeHeader::BPTreeHeader(const std::shared_ptr<BPTreeNodeManager>& nodeMan):
    BPTreePage<BPTreeHeaderStruct>(nodeMan) {

}

const BPTreeHeaderStruct * BPTreeHeader::Content() {
    auto nodeMan = _nodeMan.lock();
    return (BPTreeHeaderStruct*) nodeMan->Read(0, 0, PAGE_SIZE);
}

uint32_t BPTreeHeader::GetRoot() {
    return Content()->root;
}

void BPTreeHeader::SetRoot(uint32_t _root) {
    Update(HEADER_ROOT_OFFSET, _root);
}

uint32_t BPTreeHeader::GetFree() {
    return Content()->free;
}

void BPTreeHeader::SetFree(uint32_t _free) {
    Update(HEADER_FREE_OFFSET, _free);
}
