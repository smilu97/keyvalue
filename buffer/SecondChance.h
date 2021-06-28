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
    uint32_t _cur;
    std::unordered_map<uint32_t, uint32_t> _position;
    std::vector<uint8_t> _lifes;
public:
    explicit SecondChance(uint32_t sz);
    std::optional<uint32_t> Get(uint32_t n) override;
    std::pair<uint32_t, std::optional<uint32_t>> Load(uint32_t n) override;
};


#endif //KEYVALUE_SECONDCHANCE_H
