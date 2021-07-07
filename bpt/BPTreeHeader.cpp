//
// Created by smilu97 on 21. 6. 27..
//

#include "BPTreeHeader.h"

void BPTreeHeader::Init() {
    SetType(BPTREE_PAGE_TYPE_HEADER);
    SetRoot(0);
    SetFree(0);
    SetLength(1);
}

uint32_t BPTreeHeader::GetRoot() const {
    return Header()->root;
}

void BPTreeHeader::SetRoot(uint32_t _root) {
    Update(offsetof(BPTreeHeaderStruct, root), _root);
}

uint32_t BPTreeHeader::GetFree() const {
    return Header()->free;
}

void BPTreeHeader::SetFree(uint32_t _free) {
    Update(offsetof(BPTreeHeaderStruct, free), _free);
}

uint32_t BPTreeHeader::GetLength() const {
    return Header()->length;
}

void BPTreeHeader::SetLength(uint32_t _length) {
    Update(offsetof(BPTreeHeaderStruct, length), _length);
}
