#include "distances/ibf.hpp"
#include "distances/index_packing.hpp"
#include "commons.hpp"

#include <bitset>
#include <iostream>
#include <sstream>
#include <omp.h>

#define FMT_HEADER_ONLY
#include "fmt/format.h"

/* function return how many bits we need to store number */
inline int
bits_needed(uint64_t num)
{
    return 64 - __builtin_clzl(num);
}

typedef uint64_t block_t;
constexpr size_t block_size = sizeof(block_t) * 8;
constexpr size_t num_of_blocks = M / block_size;
typedef std::array<block_t, num_of_blocks> bitset_t;

static inline int
count_similarities(const std::vector<bitset_t> &idx_sketches, const int first, const int sec, const int min_bits)
{
    int res = M;
    for (auto bl = 0; bl < num_of_blocks; bl+= 4) {
        block_t accs[4] = {0};
        for (auto b = 0; b < min_bits; ++b) {
            accs[0] |= idx_sketches[first + b][bl] ^ idx_sketches[sec + b][bl];
            accs[1] |= idx_sketches[first + b][bl + 1] ^ idx_sketches[sec + b][bl + 1];
            accs[2] |= idx_sketches[first + b][bl + 2] ^ idx_sketches[sec + b][bl + 2];
            accs[3] |= idx_sketches[first + b][bl + 3] ^ idx_sketches[sec + b][bl + 3];
        }
        res -= __builtin_popcountll(accs[0]);
        res -= __builtin_popcountll(accs[1]);
        res -= __builtin_popcountll(accs[2]);
        res -= __builtin_popcountll(accs[3]);
    }
    return res;
}

void
dist_ibf(
    const std::vector<std::array<uint64_t, M>> sketches,
    const std::vector<std::string> files_names,
    const int num_of_threads,
    const bool edges)
{
    const size_t len_sketches = sketches.size();
    std::array<std::unordered_map<uint64_t, uint64_t>, M> mapping = hash_to_idx_each_M<uint64_t>(sketches);

    uint64_t bits_used = 0;
    for (const auto &it : mapping) {
        bits_used |= it.size();
    }

    const int min_bits = bits_needed(bits_used);  //min number of bits needed to store idxs

    std::vector<bitset_t> idx_sketches(len_sketches * min_bits, {0});

    for (auto i = 0; i < len_sketches; ++i) {
        for (auto bl = 0; bl < num_of_blocks; ++bl) {
            for (auto j = 0; j < block_size; ++j) {
                const auto m_idx = bl * block_size + j;
                const uint64_t m_val = mapping[m_idx][sketches[i][m_idx]];
                const auto sketch_idx = i * min_bits;

                for (uint64_t b = 0, mask = 1; b < min_bits; ++b, mask <<= 1) {
                    idx_sketches[sketch_idx + b][bl] |= ((m_val & mask) ? 1ULL : 0ULL) << j;
                }
            }
        }
    }
    
    if (edges) {
        // prepare vector with one string per thread
        std::vector<std::stringstream> outputs(num_of_threads);

        #pragma omp parallel for
        for (int i = 0; i < len_sketches - 1; ++i) {
            const std::string fn_w_tab = files_names[i] + '\t';
            const auto idx_i = i * min_bits;
            for(int j = i + 1; j < len_sketches; ++j) {
                const auto res = count_similarities(idx_sketches, idx_i, j * min_bits, min_bits);
                outputs[omp_get_thread_num()] << fn_w_tab << files_names[j] << '\t' << res << suffix;
            }
        }
        for (const auto &it : outputs) {
            std::cout << it.str();
        }
    } else {
    
        std::vector<fmt::memory_buffer> outputs(num_of_threads);
        print_names(files_names, len_sketches);
        #pragma omp parallel for
        for (int i = 0; i < len_sketches - 1; ++i) {
            fmt::format_to(std::back_inserter(outputs[omp_get_thread_num()]), "{}\t", files_names[i]);
            const auto idx_i = i * min_bits;
            for(int j = len_sketches - 1; j > i; --j) {
                const auto res = count_similarities(idx_sketches, idx_i, j * min_bits, min_bits);
                fmt::format_to(std::back_inserter(outputs[omp_get_thread_num()]), "{:.6}\t", res / db_m);
            }
            outputs[omp_get_thread_num()].push_back('\n');
        }

        for (const auto &it: outputs) {
            fmt::print(fmt::to_string(it));
        }
    }
}
