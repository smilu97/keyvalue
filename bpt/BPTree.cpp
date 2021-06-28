//
// Created by smilu97 on 21. 6. 27..
//

#include "BPTree.h"

#include <utility>

template <class Key, class Value>
BPTree<Key, Value>::BPTree(std::shared_ptr<BPTreeNodeManager> nodeMan): _nodeMan(std::move(nodeMan)) {

}

template <class Key, class Value>
BPTreeHeader BPTree<Key, Value>::Header() {
    return BPTreeHeader(_nodeMan);
}

template<class Key, class Value>
std::optional<BPTreeLeafNode<Key, Value>> BPTree<Key, Value>::FindLeaf(Key key) {
    auto rootPage = Header()->GetRoot();
    if (rootPage == 0) {
        return std::nullopt;
    }

    auto cur = BPTreeInternalNode<Key>(_nodeMan, rootPage);
    while (cur.IsInternal()) {
        const auto nextPage = cur.FindPage(key);
        cur = BPTreeInternalNode<Key>(_nodeMan, nextPage);
    }

    return std::make_optional(BPTreeLeafNode<Key, Value>(_nodeMan, cur.GetPage()));
}

template<class Key, class Value>
std::optional<Value*> BPTree<Key, Value>::Find(Key key) {
    std::optional<BPTreeLeafNode<Key, Value>> leaf = FindLeaf(key);
    if (false == leaf.has_value()) {
        return std::nullopt;
    }
    return leaf.value().Find(key);
}
