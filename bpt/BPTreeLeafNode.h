//
// Created by smilu97 on 21. 6. 27..
//

#ifndef KEYVALUE_BPTREELEAFNODE_H
#define KEYVALUE_BPTREELEAFNODE_H

#include <optional>

#include "BPTreePage.h"

#define LEAF_LENGTH_OFFSET (4)

struct BPTreeLeafNodeStruct {
    uint32_t type;
    uint32_t length;
};

template <class Key, class Value>
class BPTreeLeafNode: public BPTreePage<BPTreeLeafNodeStruct> {
public:
    std::optional<Value*> Find(Key key) const;
    void Init() override;
    [[nodiscard]] uint32_t GetLength() const;
private:
    Key GetNthKey(uint32_t n) const;
    const Value* GetNthValue(uint32_t n) const;
};


#endif //KEYVALUE_BPTREELEAFNODE_H
