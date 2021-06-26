//
// Created by smilu97 on 21. 6. 27..
//

#ifndef KEYVALUE_SECONDCHANCE_H
#define KEYVALUE_SECONDCHANCE_H

#include "CachePolicy.h"

#include <vector>
#include <cstdint>

class SecondChance: public CachePolicy {
    uint32_t _cur;
    std::vector<bool> _lifes;
public:
    SecondChance(uint32_t sz);
    std::pair<uint32_t, uint32_t> push(uint32_t n);
};


#endif //KEYVALUE_SECONDCHANCE_H
