#pragma once

#ifndef _MODE_H_
#define _MODE_H_

#include <vector>
#include <array>
#include "constants.hpp"

template <typename T> T mode(const std::vector<T>& vec);
std::array<uint64_t, M> get_sketch_mode(const std::vector<std::array<uint64_t, M>> input);
void
dist_mode(
    const std::vector<std::array<uint64_t, M>> sketches,
    const std::vector<std::string> files_names,
    const int num_of_threads,
    const bool edges);

#endif