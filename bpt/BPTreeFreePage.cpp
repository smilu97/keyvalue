//
// Created by smilu97 on 21. 6. 28..
//

#include "BPTreeFreePage.h"

uint32_t BPTreeFreePage::GetNext() const {
    return Header()->next;
}

void BPTreeFreePage::SetNext(uint32_t next) {
    Update(FREEPAGE_NEXT_OFFSET, next);
}

void BPTreeFreePage::Init() {
    SetType(BPTREE_PAGE_TYPE_FREE);
    SetNext(0);
}

BPTreeFreePage::BPTreeFreePage(std::shared_ptr<BPTreeNodeManager> &nodeMan, uint32_t page):
    BPTreePage<BPTreeFreePageStruct>(nodeMan, page) {

}
