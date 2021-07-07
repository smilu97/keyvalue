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
    explicit CachePolicy(uint64_t sz): _sz(sz), _rooms(sz) {}
    [[nodiscard]] uint64_t Size() const { return _sz; }
    std::optional<uint64_t> GetRoom(uint64_t index) { return _rooms[index]; }
    void SetRoom(uint64_t index, uint64_t value) { _rooms[index] = value; }

    virtual std::optional<uint64_t> Get(uint64_t n) = 0;
    virtual std::pair<uint64_t, std::optional<uint64_t>> Load(uint64_t n) = 0;
};

#endif //KEYVALUE_CACHEPOLICY_H
