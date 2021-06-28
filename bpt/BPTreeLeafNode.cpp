//
// Created by smilu97 on 21. 6. 27..
//

#include "BPTreeLeafNode.h"

template <class Key, class Value>
void BPTreeLeafNode<Key, Value>::Init() {
    SetType(BPTREE_PAGE_TYPE_LEAF);
}

template<class Key, class Value>
std::optional<Value*> BPTreeLeafNode<Key, Value>::Find(Key key) const {
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

    if (GetNthKey(left) != key)
        return std::nullopt;

    return std::make_optional(GetNthValue(left));
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
