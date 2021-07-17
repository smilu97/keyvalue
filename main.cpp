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

    std::unique_ptr<CachePolicy> policy(new SecondChance(cacheSize));
    std::shared_ptr<BPTreeNodeManager> nodeMan(new BPTreeNodeFileManager(
            filepath, std::move(policy), cacheSize
        ));
    BPTree<uint32_t, TestScheme> tree(nodeMan, branchFactorMultiplier);
    auto item = GetTestItem();

    for (int i = 0; i < 10000; i++) {
        item->value = i;
        tree.Insert(i, item);
    }

    delete item;

    tree.flush();
    
    std::cout << "Hello, World!" << std::endl;

    return 0;
}
