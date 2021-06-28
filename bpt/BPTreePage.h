//
// Created by smilu97 on 21. 6. 28..
//

#ifndef KEYVALUE_BPTREEPAGE_H
#define KEYVALUE_BPTREEPAGE_H


#include <memory>
#include "BPTreeNodeManager.h"
#include "common.h"

template <class PageStruct>
class BPTreePage {
    std::weak_ptr<BPTreeNodeManager> _nodeMan;
    uint32_t _page;
public:
    explicit BPTreePage(const std::shared_ptr<BPTreeNodeManager>& nodeMan, uint32_t page);
    bool IsHeader();
    bool IsInternal();
    bool IsLeaf();
    uint32_t GetPage() { return _page; }
    virtual void Init() = 0;
protected:
    const PageStruct * Header();
    uint32_t GetType();
    void SetType(uint32_t _type);
    void Update(uint32_t offset, uint8_t * buf, uint32_t size);
    void Update(uint32_t offset, uint8_t value);
    void Update(uint32_t offset, uint16_t value);
    void Update(uint32_t offset, uint32_t value);
    void Update(uint32_t offset, uint64_t value);
    uint8_t* Read(uint32_t offset, uint32_t size) const;
    uint8_t Read8(uint32_t offset) const;
    uint16_t Read16(uint32_t offset) const;
    uint32_t Read32(uint32_t offset) const;
    uint64_t Read64(uint32_t offset) const;

    std::weak_ptr<BPTreeNodeManager> GetNodeManager() { return _nodeMan; }
};


#endif //KEYVALUE_BPTREEPAGE_H
