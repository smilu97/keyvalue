//
// Created by smilu97 on 21. 6. 27..
//

#ifndef KEYVALUE_CACHEPOLICY_H
#define KEYVALUE_CACHEPOLICY_H

#include <cstdint>
#include <optional>
#include <vector>

class CachePolicy {
private:
    uint32_t _sz;
    std::vector< std::optional<uint32_t> > _rooms;
public:
    explicit CachePolicy(uint32_t sz): _sz(sz), _rooms(sz) {}
    [[nodiscard]] uint32_t Size() const { return _sz; }
    std::optional<uint32_t> GetRoom(uint32_t index) { return _rooms[index]; }
    void SetRoom(uint32_t index, uint32_t value) { _rooms[index] = value; }
    virtual std::optional<uint32_t> Get(uint32_t n) = 0;
    virtual std::pair<uint32_t, std::optional<uint32_t>> Load(uint32_t n) = 0;
};

#endif //KEYVALUE_CACHEPOLICY_H
