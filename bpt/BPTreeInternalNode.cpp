//
// Created by smilu97 on 21. 6. 27..
//

#include "BPTreeInternalNode.h"

template<class Key>
void BPTreeInternalNode<Key>::Init() {
    SetType(BPTREE_PAGE_TYPE_INTERNAL);
    SetLength(0);
}

template<class Key>
uint32_t BPTreeInternalNode<Key>::FindPage(Key key) const {
    const auto length = GetLength();

    uint32_t left = 0, right = length;
    while (left < right) {
        uint32_t mid = ((left + right) >> 1);
        const auto midKey = GetNthKey(mid);
        if (key >= midKey) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }

    return GetNthPage(left);
}

template<class Key>
uint32_t BPTreeInternalNode<Key>::GetLength() const {
    return Header()->length;
}

template<class Key>
void BPTreeInternalNode<Key>::SetLength(uint32_t length) {
    Update(INTERNAL_LENGTH_OFFSET, length);
}

template<class Key>
Key BPTreeInternalNode<Key>::GetNthKey(uint32_t n) const {
    const auto ptrSize = sizeof(uint32_t);
    const auto keySize = sizeof(Key);
    const uint32_t offset = (ptrSize + keySize) * n + ptrSize;
    Key key = *((Key*) Read(BPTREE_HEADER_SIZE + offset, keySize));
    return key;
}

template<class Key>
uint32_t BPTreeInternalNode<Key>::GetNthPage(uint32_t n) const {
    const auto ptrSize = sizeof(uint32_t);
    const auto keySize = sizeof(Key);
    const uint32_t offset = (ptrSize + keySize) * n;
    uint32_t key = *((uint32_t*) Read(BPTREE_HEADER_SIZE + offset, ptrSize));
    return key;
}
