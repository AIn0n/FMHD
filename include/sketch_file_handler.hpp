#pragma once

#ifndef _SKETCH_FILE_HANDLER_H_
#define _SKETCH_FILE_HANDLER_H_

#include <vector>
#include <array>
#include <string>
#include "constants.hpp"

void
save_sketches(
    const std::vector<std::array<uint64_t, M>> sketches,
    const std::vector<std::string> files_names,
    const std::string out_filename);

bool
read_sketches(
    std::vector<std::array<uint64_t, M>>& sketches,
    std::vector<std::string>& names,
    const std::string input_file);


#endif