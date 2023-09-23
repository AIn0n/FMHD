#include "distances/mode_byte_check.hpp"
#include "distances/mode.hpp"
#include "commons.hpp"

#include <iostream>
#include <sstream>
#include <omp.h>

#define FMT_HEADER_ONLY
#include "fmt/format.h"

typedef uint32_t block_t;
constexpr uint32_t block_size = sizeof(block_t) * 8;
constexpr uint64_t M_blocks = M / block_size;


static inline int
count_dif(
    const std::vector<std::array<uint64_t, M>> &sketches,
    const std::vector<std::array<block_t, M_blocks>> &bit_sketches,
    const int i,
    const int j
)
{
    int res = 0;
    for (size_t b = 0; b < M_blocks; ++b) {   // iterate thru each block
        block_t dif = ~(bit_sketches[i][b] | bit_sketches[j][b]);
        if (dif) {
            const auto block = b * block_size;
            for (int bit = 0; bit < block_size; ++bit) {
                res += (sketches[i][block + bit] == sketches[j][block + bit]);
            }
        } else {
            res +=  __builtin_popcount(bit_sketches[i][b] & bit_sketches[j][b]);
        }
    }
    return res;
}

void
dist_mode_byte_check(
    const std::vector<std::array<uint64_t, M>> sketches,
    const std::vector<std::string> files_names,
    const int num_of_threads,
    const bool edges)
{
    const size_t len_sketches = sketches.size();
    const std::array<uint64_t, M> mode = get_sketch_mode(sketches);
    std::vector<std::array<block_t, M_blocks>> bit_sketches(len_sketches);
    #pragma omp parallel for
    for (int i = 0; i < len_sketches; ++i) {
        bit_sketches[i] = get_mode_bools<block_t, M_blocks>(mode, sketches[i]);
    }

    if (edges) {
        std::vector<std::stringstream> outputs(num_of_threads);
        #pragma omp parallel for
        for (int i = 0; i < len_sketches - 1; ++i) {
            const std::string fn_w_tab = files_names[i] + '\t';
            for(int j = i + 1; j < len_sketches; ++j) {
                const int res = count_dif(sketches, bit_sketches, i, j);
                outputs[omp_get_thread_num()] << fn_w_tab << files_names[j] << '\t' << res << suffix;
            }
        }
        for (const auto &it : outputs)
            std::cout << it.str();
    } else {
        std::vector<fmt::memory_buffer> outputs(num_of_threads);
        print_names(files_names, len_sketches);
        #pragma omp parallel for
        for (int i = 0; i < len_sketches - 1; ++i) {
            fmt::format_to(std::back_inserter(outputs[omp_get_thread_num()]), "{}\t", files_names[i]);
            for(int j = len_sketches - 1; j > i; --j) {
                const int res = count_dif(sketches, bit_sketches, i, j);
                fmt::format_to(std::back_inserter(outputs[omp_get_thread_num()]), "{:.6}\t", res / db_m);
            }
            outputs[omp_get_thread_num()].push_back('\n');
        }
        for (const auto &it: outputs)
            fmt::print(fmt::to_string(it));
    }
}