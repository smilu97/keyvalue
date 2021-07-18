//
// Created by smilu97 on 21. 6. 27..
//

#ifndef KEYVALUE_BPTREE_H
#define KEYVALUE_BPTREE_H

#include "util.h"
#include "BPTreeNodeManager.h"
#include "BPTreeHeader.h"
#include "BPTreeInternalNode.h"
#include "BPTreeLeafNode.h"
#include "BPTreeFreePage.h"

#include <memory>
#include <optional>

template <class Key, class Value>
class BPTree {
    std::shared_ptr<BPTreeNodeManager> _nodeMan;
    uint32_t _internalBranchFactor;
    uint32_t _leafBranchFactor;
public:
    explicit BPTree(std::shared_ptr<BPTreeNodeManager> nodeMan, float branchFactorMultiplier = 0.8);
    std::optional<const Value*> Find(Key key) const;
    bool Erase(Key key);
    void Insert(Key key, const Value* pValue);
    void flush() const;
private:
    [[nodiscard]] BPTreeHeader Header() const;
    std::optional<BPTreeLeafNode<Key, Value>> FindLeaf(Key key) const;
    uint32_t AllocatePage();
    void FreePage(page_t page);
    uint32_t AppendPage();
};


#endif //KEYVALUE_BPTREE_H
