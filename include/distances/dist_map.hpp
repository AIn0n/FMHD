#pragma once

#ifndef _DIST_MAP_H_
#define _DIST_MAP_H_

#include <functional>
#include "distances/naive_dist.hpp"
#include "distances/mode.hpp"
#include "distances/mode_byte_check.hpp"
#include "distances/index_packing.hpp"
#include "distances/index_packing_16bits.hpp"
#include "distances/mode_trailing_zeros.hpp"
#include "distances/ibf.hpp"

const std::unordered_map<std::string, std::function<void(const std::vector<std::array<uint64_t, M>>&, const std::vector<std::string>&, const int, const bool)>> functionMap = {
    {"naive", &dist_naive},
    {"mode", &dist_mode},
    {"mode_byte_check", &dist_mode_byte_check},
    {"mode_trailing_zeros", &dist_mode_trailing_zeros},
    {"index_packing_8bits", &dist_index_packing_8bits},
    {"index_packing_16bits", &dist_index_packing_16bits},
    {"ibf", &dist_ibf}
};


#endif