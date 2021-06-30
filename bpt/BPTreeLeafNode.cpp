//
// Created by smilu97 on 21. 6. 27..
//

#include <cstring>

#include "BPTreeLeafNode.h"
#include "util.h"

template <class Key, class Value>
void BPTreeLeafNode<Key, Value>::Init() {
    SetType(BPTREE_PAGE_TYPE_LEAF);
}

template<class Key, class Value>
uint32_t BPTreeLeafNode<Key, Value>::LowerBound(Key key) const {
    const auto length = GetLength();

    uint32_t left = 0, right = length - 1;
    while (left < right) {
        const uint32_t mid = ((left + right) >> 1);
        const auto midKey = GetNthKey(mid);
        if (key <= midKey) {
            right = mid;
        } else {
            left = mid + 1;
        }
    }

    return left;
}

template<class Key, class Value>
std::optional<Value*> BPTreeLeafNode<Key, Value>::Find(Key key) const {
    const uint32_t index = LowerBound(key);
    if (GetNthKey(index) != key)
        return std::nullopt;

    return std::make_optional(GetNthValue(index));
}

template<class Key, class Value>
uint32_t BPTreeLeafNode<Key, Value>::GetLength() const {
    return Header()->length;
}

template<class Key, class Value>
Key BPTreeLeafNode<Key, Value>::GetNthKey(uint32_t n) const {
    const uint32_t offset = (sizeof(Key) + sizeof(Value)) * n;
    Key key = *((Key*) Read(BPTREE_HEADER_SIZE + offset, sizeof(Key)));
    return key;
}

template<class Key, class Value>
const Value *BPTreeLeafNode<Key, Value>::GetNthValue(uint32_t n) const {
    const uint32_t offset = (sizeof(Key) + sizeof(Value)) * n + sizeof(Key);
    const Value *pValue = (Value*) Read(BPTREE_HEADER_SIZE + offset, sizeof(Value));
    return pValue;
}

template<class Key, class Value>
void BPTreeLeafNode<Key, Value>::ShiftRight(uint32_t index) {
    const uint32_t length = GetLength();
    if (index == length) return;
    if (index > length) {
        panic("Invalid ShiftRight on InternalNode");
    }

    const size_t unit = sizeof(Key) + sizeof(Value);
    const size_t whole = unit * length;
    const size_t leftSize = unit * index;
    const size_t rightSize = whole - leftSize;
    const size_t midOffset = BPTREE_HEADER_SIZE + leftSize;

    Shift(midOffset, rightSize, unit);
    SetLength(length + 1);
}

template<class Key, class Value>
void BPTreeLeafNode<Key, Value>::ShiftLeft(uint32_t index) {
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
void BPTreeLeafNode<Key, Value>::SetLength(uint32_t length) {
    Update(LEAF_LENGTH_OFFSET, length);
}

template<class Key, class Value>
void BPTreeLeafNode<Key, Value>::Insert(Key key, const Value *pValue) {
    const auto index = LowerBound(key);
    if (index == 0)
        panic("Invalid index found");

    ShiftRight(index);
    SetKey(index, key);
    SetValue(index, pValue);
}

template<class Key, class Value>
void BPTreeLeafNode<Key, Value>::SetKey(uint32_t index, Key key) {
    const size_t unit = sizeof(Key) + sizeof(Value);
    const size_t offset = BPTREE_HEADER_SIZE + unit * index;
    Update(offset, (byte*) &key, sizeof(Key));
}

template<class Key, class Value>
void BPTreeLeafNode<Key, Value>::SetValue(uint32_t index, const Value *pValue) {
    const size_t unit = sizeof(Key) + sizeof(Value);
    const size_t offset = BPTREE_HEADER_SIZE + sizeof(Key) + unit * index;
    Update(offset, (byte*) pValue, sizeof(Value));
}

template<class Key, class Value>
std::pair<Key, BPTreeLeafNode<Key, Value>> BPTreeLeafNode<Key, Value>::Split(page_t newPage) {
    const size_t length = GetLength();
    const size_t leftLength = (length >> 1);
    const size_t rightLength = length - leftLength;
    const size_t unit = sizeof(Key) + sizeof(Value);

    const byte* rightContent = Read(BPTREE_HEADER_SIZE + leftLength * unit, rightLength * unit);
    byte* buf = new byte[rightLength * unit];
    memcpy(buf, rightContent, rightLength * unit);
    SetLength(leftLength);

    auto right = BPTreeLeafNode<Key, Value>(GetNodeManager(), newPage);
    right.Init();
    right.Update(BPTREE_HEADER_SIZE, buf, rightLength * unit);
    right.SetLength(rightLength);
    auto rightFirst = right.GetKey(0);

    delete[] buf;

    return std::make_pair(rightFirst, right);
}

template<class Key, class Value>
Key BPTreeLeafNode<Key, Value>::GetKey(offset_t index) const {
    const size_t unit = sizeof(Key) + sizeof(Value);
    return *((Key*)Read(BPTREE_HEADER_SIZE + unit * index, sizeof(Key)));
}

template<class Key, class Value>
bool BPTreeLeafNode<Key, Value>::Erase(Key key) {
    const uint32_t index = LowerBound(key);
    if (GetKey(index) != key)
        return false;

    ShiftLeft(index);
    return true;
}

template<class Key, class Value>
void BPTreeLeafNode<Key, Value>::Merge(const BPTreeLeafNode<Key, Value> &target) {
    const size_t targetLength = target.GetLength();
    const size_t unit = sizeof(Key) + sizeof(Value);
    const size_t targetSize = targetLength * unit;
    byte * buf = new byte[targetSize];
    byte * targetBuf = target.Read(BPTREE_HEADER_SIZE, targetSize);
    memcpy(buf, targetBuf, targetSize);

    const size_t length = GetLength();
    Update(BPTREE_HEADER_SIZE + length * unit, buf, targetSize);
    SetLength(length + targetLength);

    delete[] buf;
}
