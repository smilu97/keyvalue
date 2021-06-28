//
// Created by smilu97 on 21. 6. 27..
//

#include "BPTree.h"

#include <utility>

BPTree::BPTree(std::shared_ptr<BPTreeNodeManager> nodeMan): _nodeMan(std::move(nodeMan)) {

}

BPTreeHeader BPTree::Header() {
    return BPTreeHeader(_nodeMan);
}
