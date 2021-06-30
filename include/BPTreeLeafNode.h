//
// Created by smilu97 on 21. 6. 27..
//

#ifndef KEYVALUE_BPTREELEAFNODE_H
#define KEYVALUE_BPTREELEAFNODE_H

#include <optional>

#include "BPTreePage.h"

#define LEAF_LENGTH_OFFSET (4)
#define LEAF_NEXT_OFFSET (12)

struct BPTreeLeafNodeStruct {
    uint32_t type;
    uint64_t length;
    uint64_t next;
};

template <class Key, class Value>
class BPTreeLeafNode: public BPTreePage<BPTreeLeafNodeStruct> {
public:
    std::optional<Value*> Find(Key key) const;
    uint32_t LowerBound(Key key) const;
    void Init() override;
    [[nodiscard]] uint32_t GetLength() const;
    [[nodiscard]] Key GetKey(offset_t index) const;
    [[nodiscard]] page_t GetNext() const;
    void Insert(Key key, const Value * pValue);
    bool Erase(Key key);
    void Merge(const BPTreeLeafNode<Key, Value> & target);
    void Redistribute(BPTreeLeafNode<Key, Value> & target, size_t bf);
    std::pair<Key, BPTreeLeafNode<Key, Value>> Split(page_t newPage);
private:
    Key GetNthKey(uint32_t n) const;
    const Value* GetNthValue(uint32_t n) const;
    void SetLength(uint32_t length);
    void ShiftRight(uint32_t index);
    void ShiftLeft(uint32_t index);
    void SetKey(uint32_t index, Key key);
    void SetNext(page_t next);
    void SetValue(uint32_t index, const Value * pValue);
    void Append(const byte * buf, size_t length);
    void PushFront(const byte * buf, size_t length);
};


#endif //KEYVALUE_BPTREELEAFNODE_H
