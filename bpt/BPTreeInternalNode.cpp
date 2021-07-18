//
// Created by smilu97 on 21. 6. 27..
//

#include <cstring>
#include <iostream>

#include "BPTreeInternalNode.h"
#include "util.h"

template<class Key, class Value>
void BPTreeInternalNode<Key, Value>::Init() {
    SetType(BPTREE_PAGE_TYPE_INTERNAL);
    SetLength(0);
    SetNext(0);
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
    Update(offsetof(BPTreeInternalNodeStruct, length), length);
}

template<class Key, class Value>
Key BPTreeInternalNode<Key, Value>::GetNthKey(uint32_t n) const {
    const auto ptrSize = sizeof(page_t);
    const auto keySize = sizeof(Key);
    const uint32_t offset = (ptrSize + keySize) * n + ptrSize;
    Key key = *((Key*) Read(BPTREE_HEADER_SIZE + offset, keySize));
    return key;
}

template<class Key, class Value>
page_t BPTreeInternalNode<Key, Value>::GetNthPage(uint32_t n) const {
    const auto ptrSize = sizeof(page_t);
    const auto keySize = sizeof(Key);
    const offset_t offset = (ptrSize + keySize) * n;
    page_t key = *((page_t*) Read(BPTREE_HEADER_SIZE + offset, ptrSize));
    return key;
}

template<class Key, class Value>
void BPTreeInternalNode<Key, Value>::Insert(Key key, page_t page) {
    std::cout << "Internal Insert (" << key << ", " << page << ") in " << GetPage() << std::endl;
    const auto index = FindIndex(key);
    std::cout << "Internal Insert in " << index << ", length: " << GetLength() << std::endl;
    if (index == 0)
        panic("Invalid index found");

    const auto length = GetLength();
    if (length == index) {
        SetLength(length + 1);
    } else {
        ShiftRight(index);
    }

    SetKey(index, key);
    SetPtr(index+1, page);

    std::cout << "  internal " << GetPage() << " state: " << GetNthPage(0) << ' ';
    for (int i = 0; i < GetLength(); i++) {
        std::cout << GetNthKey(i) << ' ' << GetNthPage(i+1) << ' ';
    } std::cout << std::endl;
}

template<class Key, class Value>
uint32_t BPTreeInternalNode<Key, Value>::FindIndex(Key key) const {
    const auto length = GetLength();

    if (length == 0) return 0;
    if (key < GetNthKey(0)) return 0;

    uint32_t left = 0, right = length;
    while (left + 1 < right) {
        uint32_t mid = ((left + right) >> 1);
        const auto midKey = GetNthKey(mid);
        if (key >= midKey) {
            left = mid;
        } else {
            right = mid;
        }
    }
    ++left;

    std::cout << GetPage() << ": FindIndex(" << key << "): " << left << std::endl;

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
    const size_t whole = unit * length + sizeof(page_t);
    const size_t leftSize = unit * index + sizeof(page_t);
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
    const size_t rightSize = whole - leftSize - unit;
    const size_t midOffset = BPTREE_HEADER_SIZE + leftSize + unit;

    Shift(midOffset, rightSize, -unit);
    SetLength(length - 1);
}

template<class Key, class Value>
void BPTreeInternalNode<Key, Value>::SetKey(uint32_t index, Key key) {
    const size_t unit = sizeof(page_t) + sizeof(Key);
    const size_t offset = BPTREE_HEADER_SIZE + sizeof(page_t) + unit * index;
    Update(offset, (byte*) &key, sizeof(Key));
}

template<class Key, class Value>
void BPTreeInternalNode<Key, Value>::SetPtr(uint32_t index, page_t page) {
    const size_t unit = sizeof(Key) + sizeof(page_t);
    const size_t offset = BPTREE_HEADER_SIZE + unit * index;
    Update(offset, (byte*) &page, sizeof(page_t));
}

template<class Key, class Value>
void BPTreeInternalNode<Key, Value>::InsertBase(page_t p0, Key key, page_t p1) {
    SetPtr(0, p0);
    SetPtr(1, p1);
    SetKey(0, key);
    SetLength(1);
}

template<class Key, class Value>
std::pair<Key, BPTreeInternalNode<Key, Value>> BPTreeInternalNode<Key, Value>::Split(page_t newPage) {
    const size_t length = GetLength();
    const size_t leftLength = ((length - 1) >> 1);
    const size_t rightLength = (length - 1) - leftLength;
    const size_t unit = sizeof(Key) + sizeof(page_t);
    const size_t rightSize = rightLength * unit + sizeof(Key);
    const size_t rightOffset = BPTREE_HEADER_SIZE + (leftLength + 1) * unit;
    const page_t next = GetNext();
    SetNext(newPage);

    const byte* rightContent = Read(rightOffset, rightSize);
    byte* buf = new byte[rightSize];
    memcpy(buf, rightContent, rightSize);
    auto midKey = GetNthKey((length - 1) >> 1);
    SetLength(leftLength);

    auto right = BPTreeInternalNode<Key, Value>(GetNodeManager(), newPage);
    right.Init();
    right.Update(BPTREE_HEADER_SIZE, buf, rightSize);
    right.SetLength(rightLength);
    right.SetNext(next);

    delete[] buf;

    return std::make_pair(midKey, right);
}

template<class Key, class Value>
void BPTreeInternalNode<Key, Value>::Merge(Key midKey, const BPTreeInternalNode<Key, Value> &target) {
    const size_t targetLength = target.GetLength();
    const size_t unit = sizeof(Key) + sizeof(page_t);
    const size_t targetSize = targetLength * unit + sizeof(Key);
    byte * buf = new byte[targetSize];
    byte * targetBuf = target.Read(BPTREE_HEADER_SIZE, targetSize);
    memcpy(buf, targetBuf, targetSize);

    const size_t length = GetLength();
    SetKey(length, midKey);
    Update(BPTREE_HEADER_SIZE + (length + 1) * unit, buf, targetSize);
    SetLength(length + targetLength + 1);

    delete[] buf;
}

template<class Key, class Value>
page_t BPTreeInternalNode<Key, Value>::GetNext() const {
    return Header()->next;
}

template<class Key, class Value>
void BPTreeInternalNode<Key, Value>::SetNext(page_t next) {
    Update(offsetof(BPTreeInternalNodeStruct, next), next);
}

template<class Key, class Value>
page_t BPTreeInternalNode<Key, Value>::FindLeftOf(Key key) const {
    const size_t index = FindIndex(key);
    if (index == 0)
        panic("Invalid FindLeftOf call detected");

    return GetNthPage(index - 1);
}

template<class Key, class Value>
void BPTreeInternalNode<Key, Value>::EraseOf(Key key) {
    const size_t index = FindIndex(key);
    if (index == 0)
        panic("Invalid EraseOf key: First child must not be erased");
    ShiftLeft(index - 1);

    std::cout << "  internal " << GetPage() << " state: " << GetNthPage(0) << ' ';
    for (int i = 0; i < GetLength(); i++) {
        std::cout << GetNthKey(i) << ' ' << GetNthPage(i+1) << ' ';
    } std::cout << std::endl;
}

template<class Key, class Value>
Key BPTreeInternalNode<Key, Value>::Redistribute(Key midKey, BPTreeInternalNode<Key, Value> &target, size_t bf) {
    Key resultKey;
    const size_t unit = sizeof(Key) + sizeof(page_t);
    const size_t length = GetLength();
    const size_t targetLength = target.GetLength();
    if (length < bf && targetLength > bf) {
        SetKey(length, midKey);
        SetPtr(length + 1, target.GetNthPage(0));
        resultKey = target.GetNthKey(0);
        target.Shift(BPTREE_HEADER_SIZE + unit, sizeof(Key) + (unit - 1) * length, -unit);
        SetLength(length + 1);
        target.SetLength(targetLength - 1);
    } else if (length > bf && targetLength < bf) {
        target.Shift(BPTREE_HEADER_SIZE, sizeof(Key) + unit * length, unit);
        target.SetLength(targetLength + 1);
        target.SetKey(0, midKey);
        target.SetPtr(0, GetNthPage(length));
        resultKey = GetNthKey(length - 1);
        SetLength(length - 1);
    }

    return resultKey;
}

template<class Key, class Value>
void BPTreeInternalNode<Key, Value>::ReplaceKeyOf(Key key, Key newKey) {
    const offset_t index = FindIndex(key);
    if (index == 0)
        panic("Invalid ReplaceKeyOf: index == 0");

    SetKey(index - 1, newKey);
}

template<class Key, class Value>
Key BPTreeInternalNode<Key, Value>::GetFirstPage() const {
    return *((Key*) Read(BPTREE_HEADER_SIZE, sizeof(Key)));
}
