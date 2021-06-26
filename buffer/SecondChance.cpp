//
// Created by smilu97 on 21. 6. 27..
//

#include "SecondChance.h"

SecondChance::SecondChance(uint32_t sz): CachePolicy(sz), _lifes(sz), _cur(0) {

}

std::pair<uint32_t, uint32_t> SecondChance::push(uint32_t n) {
    const uint32_t sz = Size();

    while (_lifes[_cur]) {
        _lifes[_cur] = false;
        _cur = (_cur + 1) % sz;
    }
    const int victim = Get(_cur);
    Set(_cur, n);
    _lifes[_cur] = true;

    return std::make_pair(_cur, victim);
}
