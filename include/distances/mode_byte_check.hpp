#pragma once

#ifndef _MODE_BYTE_CHECK_H_
#define _MODE_BYTE_CHECK_H_

#include <vector>
#include <array>
#include "constants.hpp"

void
dist_mode_byte_check(
    const std::vector<std::array<uint64_t, M>> sketches,
    const std::vector<std::string> files_names,
    const int num_of_threads,
    const bool edges);

template <typename T, unsigned int M_blocks>
std::array<T, M_blocks>
get_mode_bools(
    const std::array<uint64_t, M>& modes,
    const std::array<uint64_t, M>& input_hashes)
{
    constexpr auto block_size = sizeof(T) * 8;
    std::array<T, M_blocks> res = {0};
    for (auto b = 0; b < M_blocks; ++b) {
        for (auto i = 0; i < block_size; ++i) {
            const auto idx = b * block_size + i;
            res[b] <<= 1;
            res[b] |= (input_hashes[idx] == modes[idx]);
            
        }
    }
    return res;
}


#endif