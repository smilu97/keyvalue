//
// Created by smilu97 on 21. 6. 27..
//

#ifndef KEYVALUE_SECONDCHANCE_H
#define KEYVALUE_SECONDCHANCE_H

#include "CachePolicy.h"

#include <vector>
#include <unordered_map>
#include <optional>
#include <cstdint>

class SecondChance: public CachePolicy {
    uint64_t _cur;
    std::unordered_map<uint64_t, uint64_t> _position;
    std::vector<uint8_t> _lifes;
public:
    explicit SecondChance(uint64_t sz);
    std::optional<uint64_t> Get(uint64_t n) override;
    std::pair<uint64_t, std::optional<uint64_t>> Load(uint64_t n) override;
};


#endif //KEYVALUE_SECONDCHANCE_H
