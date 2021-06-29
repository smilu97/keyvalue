//
// Created by smilu97 on 21. 6. 27..
//

#include "BPTree.h"

#include <utility>
#include <stack>

template <class Key, class Value>
BPTree<Key, Value>::BPTree(std::shared_ptr<BPTreeNodeManager> nodeMan, float branchFactorMultiplier):
    _nodeMan(std::move(nodeMan)) {

    const uint32_t internalBase = PAGE_SIZE - BPTREE_HEADER_SIZE - sizeof(Key);
    const uint32_t internalUnit = sizeof(Key) + sizeof(page_t);
    const uint32_t maxInternalBranchFactor = internalBase / internalUnit;
    _internalBranchFactor = maxInternalBranchFactor * branchFactorMultiplier;

    const uint32_t leafBase = PAGE_SIZE - BPTREE_HEADER_SIZE;
    const uint32_t leafUnit = sizeof(Key) + sizeof(Value);
    const uint32_t maxLeafBranchFactor = leafBase / leafUnit;
    _leafBranchFactor = maxLeafBranchFactor / branchFactorMultiplier;
}

template <class Key, class Value>
BPTreeHeader BPTree<Key, Value>::Header() const {
    return BPTreeHeader(_nodeMan);
}

template<class Key, class Value>
std::optional<BPTreeLeafNode<Key, Value>> BPTree<Key, Value>::FindLeaf(Key key) const {
    auto rootPage = Header()->GetRoot();
    if (rootPage == 0) {
        return std::nullopt;
    }

    auto cur = BPTreeInternalNode<Key, Value>(_nodeMan, rootPage);
    while (cur.IsInternal()) {
        const auto nextPage = cur.FindPage(key);
        cur = BPTreeInternalNode<Key, Value>(_nodeMan, nextPage);
    }

    return std::make_optional(BPTreeLeafNode<Key, Value>(_nodeMan, cur.GetPage()));
}

template<class Key, class Value>
std::optional<Value*> BPTree<Key, Value>::Find(Key key) const {
    std::optional<BPTreeLeafNode<Key, Value>> leaf = FindLeaf(key);
    if (false == leaf.has_value()) {
        return std::nullopt;
    }
    return leaf.value().Find(key);
}

template<class Key, class Value>
void BPTree<Key, Value>::Insert(Key key, const Value *pValue) {
    BPTreeLeafNode<Key, Value> leaf;
    uint32_t rootPage = Header()->GetRoot();

    if (rootPage == 0) {
        rootPage = AllocatePage();
        Header()->SetRoot(rootPage);
        auto root = BPTreeLeafNode<Key, Value>(_nodeMan, rootPage);
        root.Init();
        root.Insert(key, pValue);
    } else {
        std::stack<BPTreeInternalNode<Key, Value>> internals;
        auto cur = BPTreeInternalNode<Key, Value>(_nodeMan, rootPage);
        while (cur.IsInternal()) {
            internals.push(cur);
            cur = cur.FindPage(key);
        }
        auto leaf = BPTreeLeafNode<Key, Value>(_nodeMan, cur.GetPage());
        leaf.Insert(key, pValue);
        if (leaf.GetLength() > _leafBranchFactor * 2) {
            auto newLeafPage = AllocatePage();
            auto leafSplitResult = leaf.Split(newLeafPage);
            auto newKey = leafSplitResult.first;
            auto newPage = leafSplitResult.second;
            BPTreeInternalNode<Key, Value> parent;
            while (false == internals.empty()) {
                parent = internals.top();
                internals.pop();
                parent.Insert(newKey, newPage);
                if (parent.GetLength() <= _internalBranchFactor * 2) {
                    newPage = 0;
                    break;
                } else {
                    auto newInternalPage = AllocatePage();
                    auto splitResult = parent.Split(newInternalPage);
                    newKey = splitResult.first;
                    newPage = splitResult.second;
                }
            }
            if (newPage) {
                auto newRootPage = AllocatePage();
                auto newRoot = BPTreeInternalNode<Key, Value>(_nodeMan, newRootPage);
                newRoot.Init();
                newRoot.InsertBase(parent, newKey, newPage);
                Header().SetRoot(newRootPage);
            }
        }
    }
}

template<class Key, class Value>
uint32_t BPTree<Key, Value>::AllocatePage() {
    uint32_t freeHeadPage = Header()->GetFree();
    if (freeHeadPage == 0) {
        return AppendPage();
    }
    auto freeHead = BPTreeFreePage(_nodeMan, freeHeadPage);
    auto nextFreePage = freeHead.GetNext();
    Header().SetFree(nextFreePage);
    return freeHeadPage;
}

template<class Key, class Value>
uint32_t BPTree<Key, Value>::AppendPage() {
    uint32_t length = Header().GetLength();
    Header().SetLength(length + 1);
    return length;
}
