//
// Created by smilu97 on 21. 6. 27..
//

#include <cstring>

#include "BPTreeInternalNode.h"
#include "util.h"

template<class Key, class Value>
void BPTreeInternalNode<Key, Value>::Init() {
    SetType(BPTREE_PAGE_TYPE_INTERNAL);
    SetLength(0);
}

template<class Key, class Value>
page_t BPTreeInternalNode<Key, Value>::FindPage(Key key) const {
    return GetNthPage(FindIndex(key));
}

template<class Key, class Value>
uint32_t BPTreeInternalNode<Key, Value>::GetLength() const {
    return Header()->length;
}

template<class Key, class Value>
void BPTreeInternalNode<Key, Value>::SetLength(uint32_t length) {
    Update(INTERNAL_LENGTH_OFFSET, length);
}

template<class Key, class Value>
Key BPTreeInternalNode<Key, Value>::GetNthKey(uint32_t n) const {
    const auto ptrSize = sizeof(uint32_t);
    const auto keySize = sizeof(Key);
    const uint32_t offset = (ptrSize + keySize) * n + ptrSize;
    Key key = *((Key*) Read(BPTREE_HEADER_SIZE + offset, keySize));
    return key;
}

template<class Key, class Value>
uint32_t BPTreeInternalNode<Key, Value>::GetNthPage(uint32_t n) const {
    const auto ptrSize = sizeof(uint32_t);
    const auto keySize = sizeof(Key);
    const uint32_t offset = (ptrSize + keySize) * n;
    uint32_t key = *((uint32_t*) Read(BPTREE_HEADER_SIZE + offset, ptrSize));
    return key;
}

template<class Key, class Value>
void BPTreeInternalNode<Key, Value>::Insert(Key key, page_t page) {
    const auto index = FindIndex(key);
    if (index == 0)
        panic("Invalid index found");

    ShiftRight(index);
    SetKey(index, key);
    SetPtr(index, page);
}

template<class Key, class Value>
uint32_t BPTreeInternalNode<Key, Value>::FindIndex(Key key) const {
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

    return left;
}

template<class Key, class Value>
void BPTreeInternalNode<Key, Value>::ShiftRight(uint32_t index) {
    const uint32_t length = GetLength();
    if (index == length) return;
    if (index > length) {
        panic("Invalid ShiftRight on InternalNode");
    }

    const size_t unit = sizeof(Key) + sizeof(page_t);
    const size_t whole = unit * length + sizeof(Key);
    const size_t leftSize = unit * index + sizeof(Key);
    const size_t rightSize = whole - leftSize;
    const size_t midOffset = BPTREE_HEADER_SIZE + leftSize;

    Shift(midOffset, rightSize, unit);
    SetLength(length + 1);
}

template<class Key, class Value>
void BPTreeInternalNode<Key, Value>::ShiftLeft(uint32_t index) {
    const uint32_t length = GetLength();
    if (index == length) return;
    if (index > length) {
        panic("Invalid ShiftLeft on InternalNode");
    }

    const size_t unit = sizeof(Key) + sizeof(page_t);
    const size_t whole = unit * length + sizeof(Key);
    const size_t leftSize = unit * index + sizeof(Key);
    const size_t rightSize = whole - leftSize;
    const size_t midOffset = BPTREE_HEADER_SIZE + leftSize;

    Shift(midOffset, rightSize, -unit);
    SetLength(length - 1);
}

template<class Key, class Value>
void BPTreeInternalNode<Key, Value>::SetKey(uint32_t index, Key key) {
    const size_t unit = sizeof(Key) + sizeof(page_t);
    const size_t offset = BPTREE_HEADER_SIZE + unit * index;
    Update(offset, (byte*) &key, sizeof(Key));
}

template<class Key, class Value>
void BPTreeInternalNode<Key, Value>::SetPtr(uint32_t index, page_t page) {
    const size_t unit = sizeof(Key) + sizeof(page_t);
    const size_t offset = BPTREE_HEADER_SIZE + sizeof(Key) + unit * index;
    Update(offset, (byte*) &page, sizeof(page_t));
}
