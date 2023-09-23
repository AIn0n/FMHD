#pragma once

#ifndef _SKETCH_H_
#define _SKETCH_H_

#include <array>
#include <string>
#include <cstdint>
#include <math.h>
#include "constants.hpp"

const uint64_t BOTTOM_BITS = log2(M); /* bits used to first hash */
const uint64_t MASK = M - 1;

std::array<uint64_t, M>
get_sketch(
    const std::string input,
    const size_t k,
    const std::array<char, 128>& rc_lookup);


std::array<char, 128> init_rc_table(void);

#endif