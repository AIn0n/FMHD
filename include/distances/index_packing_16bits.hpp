#pragma once

#ifndef _INDEX_PACKING_16BITS_H_
#define _INDEX_PACKING_16BITS_H_

#include <vector>
#include <array>
#include "constants.hpp"

void
dist_index_packing_16bits(
    const std::vector<std::array<uint64_t, M>> sketches,
    const std::vector<std::string> files_names,
    const int num_of_threads,
    const bool edges);

#endif