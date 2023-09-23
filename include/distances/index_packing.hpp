#pragma once

#ifndef _INDEX_PACKING_H_
#define _INDEX_PACKING_H_

#include <vector>
#include <array>
#include <unordered_map>
#include "constants.hpp"

void
dist_index_packing_8bits(
    const std::vector<std::array<uint64_t, M>> sketches,
    const std::vector<std::string> files_names,
    const int num_of_threads,
    const bool edges);

// mapping for each M: hash -> new, uint8_t index
template<typename T>
std::array<std::unordered_map<uint64_t, T>, M>
hash_to_idx_each_M(const std::vector<std::array<uint64_t, M>> sketches)
{
    std::array<std::unordered_map<uint64_t, T>, M> res;
    for (int i = 0; i < M; ++i) {
        T index = 0;
        for (size_t j = 0; j < sketches.size(); ++j) {
            if(res[i].find(sketches[j][i]) == res[i].end()) {
                res[i][sketches[j][i]] = index++;
            }
        }
    }
    return res;
}

#endif