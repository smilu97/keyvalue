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

template <class Key>
class BPTreeInternalNode: public BPTreePage<BPTreeInternalNodeStruct> {
public:
    void Init() override;
    uint32_t GetLength() const;
    uint32_t FindPage(Key key) const;
private:
    void SetLength(uint32_t length);
    Key GetNthKey(uint32_t n) const;
    uint32_t GetNthPage(uint32_t n) const ;
};

#endif //KEYVALUE_BPTREEINTERNALNODE_H
