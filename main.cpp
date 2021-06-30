#include <iostream>
#include <string>

#include "bpt/BPTree.h"
#include "bpt/BPTreeNodeFileManager.h"
#include "buffer/SecondChance.h"

struct TestScheme {
    byte content[128];
};

int main() {
    const std::string filepath = "test.db";
    const uint32_t cacheSize = 1000;
    const float branchFactorMultiplier = 0.8;

    std::unique_ptr<CachePolicy> policy(new SecondChance(cacheSize));
    std::shared_ptr<BPTreeNodeManager> nodeMan(new BPTreeNodeFileManager(
            filepath, std::move(policy), cacheSize
        ));
    BPTree<uint32_t, TestScheme> tree(nodeMan, branchFactorMultiplier);
    
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
