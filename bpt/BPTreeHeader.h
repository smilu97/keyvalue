//
// Created by smilu97 on 21. 6. 27..
//

#ifndef KEYVALUE_BPTREEHEADER_H
#define KEYVALUE_BPTREEHEADER_H

#include <memory>

#include "BPTreeNodeManager.h"
#include "BPTreePage.h"
#include "common.h"

#define HEADER_ROOT_OFFSET (0)
#define HEADER_FREE_OFFSET (4)

struct BPTreeHeaderStruct {
    uint32_t root;
    uint32_t free;
};

class BPTreeHeader: public BPTreePage<BPTreeHeaderStruct> {
    std::weak_ptr<BPTreeNodeManager> _nodeMan;
public:
    explicit BPTreeHeader(const std::shared_ptr<BPTreeNodeManager>& nodeMan);
    uint32_t GetRoot();
    void SetRoot(uint32_t _root);
    uint32_t GetFree();
    void SetFree(uint32_t _free);
private:
    const BPTreeHeaderStruct * Content() override;
};


#endif //KEYVALUE_BPTREEHEADER_H
