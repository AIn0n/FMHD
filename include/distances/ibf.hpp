#pragma once

#ifndef _IBF_H_
#define _IBF_H_

#include <vector>
#include <array>
#include "constants.hpp"

void
dist_ibf(
    const std::vector<std::array<uint64_t, M>> sketches,
    const std::vector<std::string> files_names,
    const int num_of_threads,
    const bool edges);

#endif