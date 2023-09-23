#include "distances/index_packing_16bits.hpp"
#include "distances/index_packing.hpp"
#include "commons.hpp"

#include <iostream>
#include <sstream>
#include <unordered_map>
#include <omp.h>

#define FMT_HEADER_ONLY
#include "fmt/format.h"

typedef uint16_t block_t;
constexpr uint64_t split_size = sizeof(uint64_t) / sizeof(block_t);
constexpr uint64_t M_block = M / split_size;

void
dist_index_packing_16bits(
    const std::vector<std::array<uint64_t, M>> sketches,
    const std::vector<std::string> files_names,
    const int num_of_threads,
    const bool edges)
{
    const size_t len_sketches = sketches.size();
    std::array<std::unordered_map<uint64_t, block_t>, M> mapping = hash_to_idx_each_M<block_t>(sketches);
    std::vector<std::array<uint64_t, M_block>> idx_sketches(len_sketches);

    for (int j = 0; j < len_sketches; ++j) {
        block_t* idxs = (block_t *) idx_sketches[j].data();
        for (size_t i = 0; i < M; ++i) {
            idxs[i] = mapping[i][sketches[j][i]];
        }
    }

    constexpr uint64_t mask_last            = 0x7FFF7FFF7FFF7FFF;
    constexpr uint64_t one_per_two_bytes    = 0x0001000100010001;
    constexpr uint64_t get_msb              = 0x8000800080008000;

    if (edges) {
        std::vector<std::stringstream> outputs(num_of_threads);

        #pragma omp parallel for
        for (int i = 0; i < len_sketches - 1; ++i) {
            const std::string fn_w_tab = files_names[i] + '\t';
            for(int j = i + 1; j < len_sketches; ++j) {
                // iterate thru each block
                uint64_t sum = 0;
                for (int b = 0; b < M_block; b += 16) {
                    uint64_t tmp = 0;
                    for (auto bit = 0; bit < 16; ++bit) {
                        tmp |= (((~(idx_sketches[i][b + bit] ^ idx_sketches[j][b + bit]) & mask_last) + one_per_two_bytes) & get_msb) >> bit;
                    }
                    sum += __builtin_popcountll(tmp);
                }
                outputs[omp_get_thread_num()] << fn_w_tab << files_names[j] << '\t' << sum << suffix;
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
                // iterate thru each block
                uint64_t sum = 0;
                for (int b = 0; b < M_block; b += 16) {
                    uint64_t tmp = 0;
                    for (auto bit = 0; bit < 16; ++bit) {
                        tmp |= (((~(idx_sketches[i][b + bit] ^ idx_sketches[j][b + bit]) & mask_last) + one_per_two_bytes) & get_msb) >> bit;
                    }
                    sum += __builtin_popcountll(tmp);
                }
                fmt::format_to(std::back_inserter(outputs[omp_get_thread_num()]), "{:.6}\t", sum / db_m);
            }
            outputs[omp_get_thread_num()].push_back('\n');
        }
        for (const auto &it: outputs)
            fmt::print(fmt::to_string(it));
    }
}