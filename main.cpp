#include <iostream>
#include <string>
#include <cstring>
#include <fcntl.h>
#include <armadillo>

#include "include/BPTree.h"
#include "include/BPTreeNodeFileManager.h"
#include "include/SecondChance.h"

struct TestScheme {
    int value;
    byte content[128];
};

TestScheme* GetTestItem() {
    auto res = new TestScheme;
    memset(res->content, 'A', 128);
    return res;
}

int main() {
    const std::string filepath = "test.db";
    const uint32_t cacheSize = 1000;
    const float branchFactorMultiplier = 0.8;

    unlink("test.db");

    std::unique_ptr<CachePolicy> policy(new SecondChance(cacheSize));
    std::shared_ptr<BPTreeNodeManager> nodeMan(new BPTreeNodeFileManager(
            filepath, std::move(policy), cacheSize
        ));
    BPTree<uint32_t, TestScheme> tree(nodeMan, branchFactorMultiplier);
    auto item = GetTestItem();
    const int testLength = 40;

    for (int i = 0; i < testLength; i++) {
        item->value = i;
        tree.Insert(i, item);
    }

    for (int i = 0; i < testLength; i++) {
        if (i & 1) tree.Erase(i);
    }

    for (int i = 0; i < testLength; i++) {
        if (i & 1) continue;
        std::cout << "Check " << i << " existence" << std::endl;
        auto res = tree.Find(i);
        if (!res.has_value()) {
            std::cout << "not found " << i << std::endl;
        } else if (res.value()->value != i) {
            std::cout << "mismatch: " << i << std::endl;
        }
    }

    delete item;

    tree.flush();

    return 0;
}
