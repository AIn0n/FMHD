#pragma once

#ifndef _MODE_TRAILING_ZEROS_H_
#define _MODE_TRAILING_ZEROS_H_


#include <vector>
#include <array>
#include "constants.hpp"

void
dist_mode_trailing_zeros(
    const std::vector<std::array<uint64_t, M>> sketches,
    const std::vector<std::string> files_names,
    const int num_of_threads,
    const bool edges);

#endif