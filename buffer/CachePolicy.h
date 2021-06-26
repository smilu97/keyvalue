//
// Created by smilu97 on 21. 6. 27..
//

#ifndef KEYVALUE_CACHEPOLICY_H
#define KEYVALUE_CACHEPOLICY_H

#include <cstdint>
#include <vector>

class CachePolicy {
private:
    uint32_t _sz;
    std::vector<uint32_t> _rooms;
public:
    CachePolicy(uint32_t sz): _sz(sz), _rooms(sz) {}
    uint32_t Size() { return _sz; }
    uint32_t Get(uint32_t index) { return _rooms[index]; }
    void Set(uint32_t index, uint32_t value) { _rooms[index] = value; }
    virtual std::pair<uint32_t, uint32_t> push(uint32_t n) = 0;
};

#endif //KEYVALUE_CACHEPOLICY_H
