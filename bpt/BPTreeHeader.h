//
// Created by smilu97 on 21. 6. 27..
//

#ifndef KEYVALUE_BPTREEHEADER_H
#define KEYVALUE_BPTREEHEADER_H

#include <memory>

#include "BPTreeNodeManager.h"
#include "BPTreePage.h"
#include "common.h"

#define HEADER_ROOT_OFFSET (4)
#define HEADER_FREE_OFFSET (8)
#define HEADER_LENGTH_OFFSET (12)

struct BPTreeHeaderStruct {
    uint32_t type;
    uint32_t root;
    uint32_t free;
    uint32_t length;
};

class BPTreeHeader: public BPTreePage<BPTreeHeaderStruct> {
    std::weak_ptr<BPTreeNodeManager> _nodeMan;
public:
    void Init() override;
    [[nodiscard]] uint32_t GetRoot() const;
    void SetRoot(uint32_t _root);
    [[nodiscard]] uint32_t GetFree() const;
    void SetFree(uint32_t _free);
    [[nodiscard]] uint32_t GetLength() const;
    void SetLength(uint32_t _length);
};


#endif //KEYVALUE_BPTREEHEADER_H
