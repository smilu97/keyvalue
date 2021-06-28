//
// Created by smilu97 on 21. 6. 27..
//

#ifndef KEYVALUE_BPTREE_H
#define KEYVALUE_BPTREE_H

#include "BPTreeNodeManager.h"
#include "BPTreeHeader.h"

#include <memory>

class BPTree {
    std::shared_ptr<BPTreeNodeManager> _nodeMan;
public:
    explicit BPTree(std::shared_ptr<BPTreeNodeManager> nodeMan);

private:
    BPTreeHeader Header();
};


#endif //KEYVALUE_BPTREE_H
