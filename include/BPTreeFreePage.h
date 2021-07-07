//
// Created by smilu97 on 21. 6. 28..
//

#ifndef KEYVALUE_BPTREEFREEPAGE_H
#define KEYVALUE_BPTREEFREEPAGE_H

#include <utility>

#include "BPTreePage.h"

struct BPTreeFreePageStruct {
    uint32_t type;
    uint32_t next;
};

class BPTreeFreePage: public BPTreePage<BPTreeFreePageStruct> {
public:
    explicit BPTreeFreePage(std::weak_ptr<BPTreeNodeManager> nodeMan, page_t page = 0):
        BPTreePage(std::move(nodeMan), page) {}

    void Init() override;
    [[nodiscard]] uint32_t GetNext() const;
    void SetNext(uint32_t next);
};


#endif //KEYVALUE_BPTREEFREEPAGE_H
