//
// Created by smilu97 on 21. 6. 27..
//

#include <cstring>

#include "BPTreeLeafNode.h"
#include "util.h"

template <class Key, class Value>
void BPTreeLeafNode<Key, Value>::Init() {
    SetType(BPTREE_PAGE_TYPE_LEAF);
    SetLength(0);
    SetNext(0);
}

template<class Key, class Value>
uint32_t BPTreeLeafNode<Key, Value>::LowerBound(Key key) const {
    const auto length = GetLength();

    if (length == 0) return 0;

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
uint32_t BPTreeLeafNode<Key, Value>::UpperBound(Key key) const {
    const auto length = GetLength();

    if (length == 0) return 0;

    uint32_t left = 0, right = length;
    while (left < right) {
        const uint32_t mid = ((left + right) >> 1);
        const auto midKey = GetNthKey(mid);
        if (key < midKey) {
            right = mid;
        } else {
            left = mid + 1;
        }
    }

    return left;
}

template<class Key, class Value>
std::optional<const Value*> BPTreeLeafNode<Key, Value>::Find(Key key) const {
    const uint32_t index = LowerBound(key);
    std::cout << "Leaf Find " << key << " index: " << index << std::endl;
    std::cout << "GetNthKey(" << index << "): " << GetNthKey(index) << std::endl;
    if (GetNthKey(index) != key)
        return std::nullopt;

    return std::make_optional(GetNthValue(index));
}

template<class Key, class Value>
size_t BPTreeLeafNode<Key, Value>::GetLength() const {
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

    const size_t unit = sizeof(Key) + sizeof(Value);
    const size_t whole = unit * length;
    const size_t leftSize = unit * index;
    const size_t rightSize = whole - leftSize - unit;
    const size_t midOffset = BPTREE_HEADER_SIZE + leftSize + unit;

    Shift(midOffset, rightSize, -unit);
    SetLength(length - 1);
}

template<class Key, class Value>
void BPTreeLeafNode<Key, Value>::SetLength(size_t length) {
    Update(offsetof(BPTreeLeafNodeStruct, length), length);
}

template<class Key, class Value>
void BPTreeLeafNode<Key, Value>::Insert(Key key, const Value *pValue) {
    std::cout << "Leaf Insert " << key << " in " << GetPage() << std::endl;
    const auto index = UpperBound(key);
    std::cout << "  index: " << index << std::endl;
    const auto length = GetLength();

    if (index < length) ShiftRight(index);
    SetKey(index, key);
    SetValue(index, pValue);
    if (index == length) {
        SetLength(length + 1);
    }
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
    const page_t next = GetNext();
    SetNext(newPage);

    const byte* rightContent = Read(BPTREE_HEADER_SIZE + leftLength * unit, rightLength * unit);
    byte* buf = new byte[rightLength * unit];
    memcpy(buf, rightContent, rightLength * unit);
    SetLength(leftLength);

    auto right = BPTreeLeafNode<Key, Value>(GetNodeManager(), newPage);
    right.Init();
    right.Update(BPTREE_HEADER_SIZE, buf, rightLength * unit);
    right.SetLength(rightLength);
    right.SetNext(next);
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

    std::cout << "Leaf Erase " << key << " in " << index << std::endl;
    std::cout << "  leaf state: ";
    for (int i = 0; i < GetLength(); i++) {
        std::cout << GetNthKey(i) << ' ';
    } std::cout << std::endl;

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
    Append(buf, targetLength);

    delete[] buf;
}

template<class Key, class Value>
void BPTreeLeafNode<Key, Value>::Append(const byte *buf, size_t length) {
    const size_t unit = sizeof(Key) + sizeof(Value);
    const size_t currentLength = GetLength();
    const size_t offset = BPTREE_HEADER_SIZE + currentLength * unit;
    const size_t size = length * unit;
    Update(offset, buf, size);
    SetLength(currentLength + length);
}

template<class Key, class Value>
void BPTreeLeafNode<Key, Value>::PushFront(const byte *buf, size_t length) {
    const size_t unit = sizeof(Key) + sizeof(Value);
    const size_t currentLength = GetLength();

    byte * currentBuf = new byte[currentLength * unit];
    const byte * current = Read(BPTREE_HEADER_SIZE, currentLength * unit);
    memcpy(currentBuf, current, currentLength * unit);
    Update(BPTREE_HEADER_SIZE + length * unit, currentBuf, currentLength * unit);
    Update(BPTREE_HEADER_SIZE, buf, length * unit);
    SetLength(currentLength + length);
}

template<class Key, class Value>
void BPTreeLeafNode<Key, Value>::Redistribute(BPTreeLeafNode<Key, Value> &target, size_t bf) {
    const size_t unit = sizeof(Key) + sizeof(Value);
    const size_t length = GetLength();
    const size_t targetLength = target.GetLength();
    if (length < bf && targetLength > bf) {
        byte * buf = new byte[unit];
        memcpy(buf, target.Read(BPTREE_HEADER_SIZE, unit), unit);
        Append(buf, 1);
        target.ShiftLeft(0);
        delete[] buf;
    } else if (length > bf && targetLength < bf) {
        byte * buf = new byte[unit];
        memcpy(buf, Read(BPTREE_HEADER_SIZE + (length - 1) * unit, unit), unit);
        target.PushFront(buf, 1);
        SetLength(length - 1);
        delete[] buf;
    }
}

template<class Key, class Value>
page_t BPTreeLeafNode<Key, Value>::GetNext() const {
    return Header()->next;
}

template<class Key, class Value>
void BPTreeLeafNode<Key, Value>::SetNext(page_t next) {
    Update(offsetof(BPTreeLeafNodeStruct, next), next);
}
