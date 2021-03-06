//
// Created by smilu97 on 21. 6. 27..
//

#include "SecondChance.h"

SecondChance::SecondChance(uint64_t sz): CachePolicy(sz), _lifes(sz), _cur(0) {

}

std::optional<uint64_t> SecondChance::Get(uint64_t n) {
    auto it = _position.find(n);
    if (it == _position.end()) {
        return std::nullopt;
    }
    _lifes[it->second] = 1;
    return it->second;
}

std::pair<uint64_t, std::optional<uint64_t>> SecondChance::Load(uint64_t n) {
    const uint64_t sz = Size();

    while (_lifes[_cur]) {
        _lifes[_cur] = 0;
        _cur = (_cur + 1) % sz;
    }
    const auto victim = GetRoom(_cur);
    SetRoom(_cur, n);
    _lifes[_cur] = 1;

    _position.insert(std::make_pair(n, _cur));
    if (victim.has_value())
        _position.erase(victim.value());

    return std::make_pair(_cur, victim);
}
