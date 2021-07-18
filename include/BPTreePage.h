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
    page_t _page;
public:
    explicit BPTreePage(std::weak_ptr<BPTreeNodeManager> nodeMan, page_t page);
    bool IsHeader();
    bool IsInternal();
    bool IsLeaf();
    page_t GetPage() const { return _page; }
    virtual void Init() = 0;
protected:
    const PageStruct * Header() const;
    uint32_t GetType();
    void SetType(uint32_t _type);
    void Update(offset_t offset, const byte * buf, size_t size);
    void Update(offset_t offset, uint8_t value);
    void Update(offset_t offset, uint16_t value);
    void Update(offset_t offset, uint32_t value);
    void Update(offset_t offset, uint64_t value);
    void Shift(offset_t from, size_t size, offset_t offset);
    [[nodiscard]] byte* Read(offset_t offset, size_t size) const;
    [[nodiscard]] uint8_t Read8(offset_t offset) const;
    [[nodiscard]] uint16_t Read16(offset_t offset) const;
    [[nodiscard]] uint32_t Read32(offset_t offset) const;
    [[nodiscard]] uint64_t Read64(offset_t offset) const;

    std::weak_ptr<BPTreeNodeManager> GetNodeManager() { return _nodeMan; }
};


#endif //KEYVALUE_BPTREEPAGE_H
