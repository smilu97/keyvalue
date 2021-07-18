//
// Created by smilu97 on 21. 6. 27..
//

#include "BPTree.h"

#include <utility>
#include <stack>

template <class Key, class Value>
BPTree<Key, Value>::BPTree(std::shared_ptr<BPTreeNodeManager> nodeMan, float branchFactorMultiplier):
    _nodeMan(std::move(nodeMan)) {

    const uint32_t internalBase = (PAGE_SIZE - BPTREE_HEADER_SIZE - sizeof(Key)) >> 1;
    const uint32_t internalUnit = sizeof(Key) + sizeof(page_t);
    const uint32_t maxInternalBranchFactor = internalBase / internalUnit;
    _internalBranchFactor = (uint32_t) ((float) maxInternalBranchFactor * branchFactorMultiplier);

    const uint32_t leafBase = (PAGE_SIZE - BPTREE_HEADER_SIZE) >> 1;
    const uint32_t leafUnit = sizeof(Key) + sizeof(Value);
    const uint32_t maxLeafBranchFactor = leafBase / leafUnit;
    _leafBranchFactor = (uint32_t) ((float)maxLeafBranchFactor * branchFactorMultiplier);
}

template <class Key, class Value>
BPTreeHeader BPTree<Key, Value>::Header() const {
    return BPTreeHeader(_nodeMan);
}

template<class Key, class Value>
std::optional<BPTreeLeafNode<Key, Value>> BPTree<Key, Value>::FindLeaf(Key key) const {
    auto rootPage = Header().GetRoot();
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
std::optional<const Value*> BPTree<Key, Value>::Find(Key key) const {
    std::optional<BPTreeLeafNode<Key, Value>> leaf = FindLeaf(key);
    if (false == leaf.has_value()) {
        return std::nullopt;
    }
    return leaf.value().Find(key);
}

template<class Key, class Value>
void BPTree<Key, Value>::Insert(Key key, const Value *pValue) {
    page_t rootPage = Header().GetRoot();

    if (rootPage == 0) {
        rootPage = AllocatePage();
        Header().SetRoot(rootPage);
        auto root = BPTreeLeafNode<Key, Value>(_nodeMan, rootPage);
        root.Init();
        root.Insert(key, pValue);
    } else {
        std::stack<BPTreeInternalNode<Key, Value>> internals;
        auto cur = BPTreeInternalNode<Key, Value>(_nodeMan, rootPage);
        while (cur.IsInternal()) {
            internals.push(cur);
            cur = BPTreeInternalNode<Key, Value>(_nodeMan, cur.FindPage(key));
        }
        auto leaf = BPTreeLeafNode<Key, Value>(_nodeMan, cur.GetPage());
        leaf.Insert(key, pValue);
        if (leaf.GetLength() > _leafBranchFactor * 2) {
            std::cout << "Split leaf occured, length: " << leaf.GetLength() <<   std::endl;
            auto newLeafPage = AllocatePage();
            auto leafSplitResult = leaf.Split(newLeafPage);
            std::cout << "Split " << newLeafPage << " from " << leaf.GetPage() << std::endl;
            std::cout << "  left leaf length: " << leaf.GetLength() << std::endl;
            std::cout << "  right leaf length: " << leafSplitResult.second.GetLength() << std::endl;
            auto newKey = leafSplitResult.first;
            auto newPage = leafSplitResult.second.GetPage();
            bool splitRoot = true;
            page_t parentPage = leaf.GetPage();
            while (false == internals.empty()) {
                auto parent = internals.top();
                parentPage = parent.GetPage();
                internals.pop();
                parent.Insert(newKey, newPage);
                if (parent.GetLength() <= _internalBranchFactor * 2) {
                    splitRoot = false;
                    break;
                } else {
                    auto newInternalPage = AllocatePage();
                    auto splitResult = parent.Split(newInternalPage);
                    newKey = splitResult.first;
                    newPage = splitResult.second.GetPage();
                }
            }
            if (splitRoot) {
                std::cout << "Split root" << std::endl;
                auto newRootPage = AllocatePage();
                auto newRoot = BPTreeInternalNode<Key, Value>(_nodeMan, newRootPage);
                newRoot.Init();
                newRoot.InsertBase(parentPage, newKey, newPage);
                Header().SetRoot(newRootPage);
                std::cout << "  new root: " << newRootPage << std::endl;
            }
        }
    }
}

template<class Key, class Value>
uint32_t BPTree<Key, Value>::AllocatePage() {
    auto header = Header();
    uint32_t freeHeadPage = header.GetFree();
    if (freeHeadPage == 0) {
        return AppendPage();
    }
    auto freeHead = BPTreeFreePage(_nodeMan, freeHeadPage);
    auto nextFreePage = freeHead.GetNext();
    header.SetFree(nextFreePage);
    return freeHeadPage;
}

template<class Key, class Value>
uint32_t BPTree<Key, Value>::AppendPage() {
    uint32_t length = Header().GetLength();
    if (length == 0) length = 1;
    Header().SetLength(length + 1);
    return length;
}

template<class Key, class Value>
bool BPTree<Key, Value>::Erase(Key key) {
    uint32_t rootPage = Header().GetRoot();

    if (rootPage == 0) {
        return false;
    } else {
        std::stack<BPTreeInternalNode<Key, Value>> internals;
        auto cur = BPTreeInternalNode<Key, Value>(_nodeMan, rootPage);
        while (cur.IsInternal()) {
            internals.push(cur);
            cur = BPTreeInternalNode<Key, Value>(_nodeMan, cur.FindPage(key));
        }
        auto leaf = BPTreeLeafNode<Key, Value>(_nodeMan, cur.GetPage());
        if (false == leaf.Erase(key))
            return false;

        if (leaf.GetLength() < _leafBranchFactor) {
            if (rootPage == leaf.GetPage())
                return true;

            page_t nextPage = leaf.GetNext();
            BPTreeLeafNode<Key, Value> nextLeaf(_nodeMan, 0); // temporary initialization, this must be updated and used
            if (nextPage == 0) {
                const page_t leftPage = internals.top().FindLeftOf(key);
                nextLeaf = BPTreeLeafNode<Key, Value>(_nodeMan, leftPage);
                std::swap(leaf, nextLeaf);
            } else {
                nextLeaf = BPTreeLeafNode<Key, Value>(_nodeMan, nextPage);
            }

            if (leaf.GetLength() == _leafBranchFactor
                || nextLeaf.GetLength() == _leafBranchFactor) {
                leaf.Merge(nextLeaf);
                internals.top().EraseOf(nextLeaf.GetKey(0));
                FreePage(nextLeaf.GetPage());
            } else {
                leaf.Redistribute(nextLeaf, _leafBranchFactor);
                return true;
            }

            while (false == internals.empty()
                && internals.top().GetLength() < _internalBranchFactor) {

                BPTreeInternalNode<Key, Value> curr = internals.top();
                BPTreeInternalNode<Key, Value> nextInternal = internals.top();
                internals.pop();

                if (internals.empty())
                    break;

                BPTreeInternalNode<Key, Value> parent = internals.top();

                if (curr.GetNext() == 0) {
                    nextInternal = BPTreeInternalNode<Key, Value>(_nodeMan, parent.FindLeftOf(key));
                    std::swap(curr, nextInternal);
                } else {
                    nextInternal = BPTreeInternalNode<Key, Value>(_nodeMan, curr.GetNext());
                }

                const Key midKey = parent.GetNthKey(parent.FindIndex(curr.GetNthKey(0)));
                if (curr.GetLength() == _internalBranchFactor
                 || nextInternal.GetLength() == _internalBranchFactor) {
                    curr.Merge(midKey, nextInternal);
                    FreePage(nextInternal.GetPage());
                    parent.EraseOf(nextInternal.GetNthKey(0));
                } else {
                    Key nextFirstKey = nextInternal.GetNthKey(0);
                    const Key newMidKey = curr.Redistribute(midKey, nextInternal, _internalBranchFactor);
                    parent.ReplaceKeyOf(nextFirstKey, newMidKey);
                    return true;
                }
            }
        }
    }

    auto internalRoot = BPTreeInternalNode<Key, Value>(_nodeMan, rootPage);
    if (internalRoot.IsInternal() && internalRoot.GetLength() == 0) {
        const page_t nextRoot = internalRoot.GetFirstPage();
        FreePage(rootPage);
        std::cout << "New root by erase: " << nextRoot << std::endl;
        Header().SetRoot(nextRoot);
    }

    return true;
}

template<class Key, class Value>
void BPTree<Key, Value>::FreePage(page_t page) {
    auto freeHead = Header().GetFree();
    auto freePage = BPTreeFreePage(_nodeMan, page);
    freePage.Init();
    freePage.SetNext(freeHead);
    Header().SetFree(page);
}

template<class Key, class Value>
void BPTree<Key, Value>::flush() const {
    _nodeMan->flush();
}
