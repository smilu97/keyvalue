//
// Created by smilu97 on 21. 6. 27..
//

#ifndef KEYVALUE_BPTREEINTERNALNODE_H
#define KEYVALUE_BPTREEINTERNALNODE_H

#include <cstdint>

#include "BPTreePage.h"
#include "BPTreeLeafNode.h"
#include "common.h"

struct BPTreeInternalNodeStruct {
    uint32_t type;
    uint64_t length;
    uint64_t next;
};

template <class Key, class Value>
class BPTreeInternalNode: public BPTreePage<BPTreeInternalNodeStruct> {
public:
    explicit BPTreeInternalNode(std::weak_ptr<BPTreeNodeManager> nodeMan, page_t page):
            BPTreePage(nodeMan, page) {}
    void Init() override;
    [[nodiscard]] uint32_t GetLength() const;
    [[nodiscard]] page_t GetNext() const;
    [[nodiscard]] Key GetFirstPage() const;
    [[nodiscard]] Key GetNthKey(uint32_t n) const;
    page_t FindPage(Key key) const;
    page_t FindLeftOf(Key key) const;
    uint32_t FindIndex(Key key) const;
    void EraseOf(Key key);
    void InsertBase(page_t p0, Key key, page_t p1);
    void Insert(Key key, page_t page);
    void Merge(Key midKey, const BPTreeInternalNode<Key, Value> & target);
    Key Redistribute(Key midKey, BPTreeInternalNode<Key, Value> & target, size_t bf);
    void ReplaceKeyOf(Key key, Key newKey);
    std::pair<Key, BPTreeInternalNode<Key, Value>> Split(page_t newPage);
private:
    void SetLength(uint32_t length);
    void SetNext(page_t next);
    [[nodiscard]] page_t GetNthPage(uint32_t n) const;
    void ShiftRight(uint32_t index);
    void ShiftLeft(uint32_t index);
    void SetKey(uint32_t index, Key key);
    void SetPtr(uint32_t index, page_t page);
};

#endif //KEYVALUE_BPTREEINTERNALNODE_H
