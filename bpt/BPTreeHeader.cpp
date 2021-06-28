//
// Created by smilu97 on 21. 6. 27..
//

#include "BPTreeHeader.h"

void BPTreeHeader::Init() {
    SetType(BPTREE_PAGE_TYPE_HEADER);
    SetRoot(0);
    SetFree(0);
}

uint32_t BPTreeHeader::GetRoot() {
    return Header()->root;
}

void BPTreeHeader::SetRoot(uint32_t _root) {
    Update(HEADER_ROOT_OFFSET, _root);
}

uint32_t BPTreeHeader::GetFree() {
    return Header()->free;
}

void BPTreeHeader::SetFree(uint32_t _free) {
    Update(HEADER_FREE_OFFSET, _free);
}
