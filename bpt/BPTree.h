//
// Created by smilu97 on 21. 6. 27..
//

#ifndef KEYVALUE_BPTREE_H
#define KEYVALUE_BPTREE_H

#include "BPTreeNodeManager.h"
#include "BPTreeHeader.h"
#include "BPTreeInternalNode.h"
#include "BPTreeLeafNode.h"

#include <memory>
#include <optional>

template <class Key, class Value>
class BPTree {
    std::shared_ptr<BPTreeNodeManager> _nodeMan;
public:
    explicit BPTree(std::shared_ptr<BPTreeNodeManager> nodeMan);
    std::optional<Value*> Find(Key key);
private:
    BPTreeHeader Header();
    std::optional<BPTreeLeafNode<Key, Value>> FindLeaf(Key key);
};


#endif //KEYVALUE_BPTREE_H
