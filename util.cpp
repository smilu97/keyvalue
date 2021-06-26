//
// Created by smilu97 on 21. 6. 27..
//

#include "util.h"

void panic(const std::string & message) {
    std::cerr << message;
    exit(-1);
}
