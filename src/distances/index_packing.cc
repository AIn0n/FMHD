#include "distances/index_packing.hpp"
#include "distances/index_packing_16bits.hpp"
#include "commons.hpp"

#include <iostream>
#include <sstream>
#include <unordered_map>
#include <omp.h>

#define FMT_HEADER_ONLY
#include "fmt/format.h"

typedef uint8_t block_t;
constexpr uint64_t split_size = sizeof(uint64_t) / sizeof(block_t);
constexpr uint64_t M_block = M / split_size;


static inline uint64_t count_similarities(const std::array<uint64_t, M_block> &first, const std::array<uint64_t, M_block> &second)
{
    constexpr uint64_t mask_last    = 0x7F7F7F7F7F7F7F7F;
    constexpr uint64_t one_per_byte = 0x0101010101010101;
    constexpr uint64_t get_msb      = 0x8080808080808080;
    
    uint64_t sum = 0;
    for (int b = 0; b < M_block; b += 8) {
        uint64_t tmp = ((~(first[b] ^ second[b]) & mask_last) + one_per_byte) & get_msb;
        tmp |= (((~(first[b + 1] ^ second[b + 1]) & mask_last) + one_per_byte) & get_msb) >> 1;
        tmp |= (((~(first[b + 2] ^ second[b + 2]) & mask_last) + one_per_byte) & get_msb) >> 2;
        tmp |= (((~(first[b + 3] ^ second[b + 3]) & mask_last) + one_per_byte) & get_msb) >> 3;
        tmp |= (((~(first[b + 4] ^ second[b + 4]) & mask_last) + one_per_byte) & get_msb) >> 4;
        tmp |= (((~(first[b + 5] ^ second[b + 5]) & mask_last) + one_per_byte) & get_msb) >> 5;
        tmp |= (((~(first[b + 6] ^ second[b + 6]) & mask_last) + one_per_byte) & get_msb) >> 6;
        tmp |= (((~(first[b + 7] ^ second[b + 7]) & mask_last) + one_per_byte) & get_msb) >> 7;
        sum += __builtin_popcountll(tmp);
    }
    return sum;
}

void
dist_index_packing_8bits(
    const std::vector<std::array<uint64_t, M>> sketches,
    const std::vector<std::string> files_names,
    const int num_of_threads,
    const bool edges)
{
    const size_t len_sketches = sketches.size();
    std::array<std::unordered_map<uint64_t, block_t>, M> mapping = hash_to_idx_each_M<block_t>(sketches);

    for (const auto &m : mapping) {
        if (m.size() > 127)
            return dist_index_packing_16bits(sketches, files_names, num_of_threads, edges);
    }

    std::vector<std::array<uint64_t, M_block>> idx_sketches(len_sketches);

    for (int j = 0; j < len_sketches; ++j) {
        block_t* idxs = (block_t *) idx_sketches[j].data();
        for (size_t i = 0; i < M; ++i) {
            idxs[i] = mapping[i][sketches[j][i]];
        }
    }

    if (edges) {
        std::vector<std::stringstream> outputs(num_of_threads);
        #pragma omp parallel for
        for (int i = 0; i < len_sketches - 1; ++i) {
            const std::string fn_w_tab = files_names[i] + '\t';
            for(int j = i + 1; j < len_sketches; ++j) {
                const uint64_t sum = count_similarities(idx_sketches[i], idx_sketches[j]);
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
                const uint64_t sum = count_similarities(idx_sketches[i], idx_sketches[j]);
                fmt::format_to(std::back_inserter(outputs[omp_get_thread_num()]), "{:.6}\t", sum / db_m);
            }
            outputs[omp_get_thread_num()].push_back('\n');
        }
        for (const auto &it: outputs)
            fmt::print(fmt::to_string(it));
    }
}
