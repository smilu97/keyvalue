//
// Created by smilu97 on 21. 6. 28..
//

#ifndef KEYVALUE_BPTREEFREEPAGE_H
#define KEYVALUE_BPTREEFREEPAGE_H

#include "BPTreePage.h"

#define FREEPAGE_NEXT_OFFSET (4)

struct BPTreeFreePageStruct {
    uint32_t type;
    uint32_t next;
};

class BPTreeFreePage: public BPTreePage<BPTreeFreePageStruct> {
public:
    BPTreeFreePage(std::shared_ptr<BPTreeNodeManager> &nodeMan, uint32_t page);

    void Init() override;
    [[nodiscard]] uint32_t GetNext() const;
    void SetNext(uint32_t next);
};


#endif //KEYVALUE_BPTREEFREEPAGE_H
