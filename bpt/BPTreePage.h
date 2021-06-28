//
// Created by smilu97 on 21. 6. 28..
//

#ifndef KEYVALUE_BPTREEPAGE_H
#define KEYVALUE_BPTREEPAGE_H


#include <memory>
#include "BPTreeNodeManager.h"

template <class PageStruct>
class BPTreePage {
    std::weak_ptr<BPTreeNodeManager> _nodeMan;
public:
    explicit BPTreePage(const std::shared_ptr<BPTreeNodeManager>& nodeMan);
protected:
    virtual const PageStruct * Content() = 0;
    void Update(uint32_t offset, uint8_t * buf, uint32_t size);
    void Update(uint32_t offset, uint8_t value);
    void Update(uint32_t offset, uint16_t value);
    void Update(uint32_t offset, uint32_t value);
    void Update(uint32_t offset, uint64_t value);
};


#endif //KEYVALUE_BPTREEPAGE_H
