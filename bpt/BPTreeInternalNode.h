//
// Created by smilu97 on 21. 6. 27..
//

#ifndef KEYVALUE_BPTREEINTERNALNODE_H
#define KEYVALUE_BPTREEINTERNALNODE_H

#include <cstdint>

#include "BPTreePage.h"
#include "BPTreeLeafNode.h"
#include "common.h"

#define INTERNAL_LENGTH_OFFSET (4)

struct BPTreeInternalNodeStruct {
    uint32_t type;
    uint32_t length;
};

template <class Key, class Value>
class BPTreeInternalNode: public BPTreePage<BPTreeInternalNodeStruct> {
public:
    void Init() override;
    [[nodiscard]] uint32_t GetLength() const;
    page_t FindPage(Key key) const;
    void Insert(Key key, page_t page);
private:
    uint32_t FindIndex(Key key) const;
    void SetLength(uint32_t length);
    Key GetNthKey(uint32_t n) const;
    [[nodiscard]] uint32_t GetNthPage(uint32_t n) const;
    void ShiftRight(uint32_t index);
    void ShiftLeft(uint32_t index);
    void SetKey(uint32_t index, Key key);
    void SetPtr(uint32_t index, page_t page);
};

#endif //KEYVALUE_BPTREEINTERNALNODE_H
