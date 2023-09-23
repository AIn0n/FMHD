#include "distances/mode.hpp"
#include "commons.hpp"

#include <bitset> /* storing vec[i] == mode values */
#include <unordered_map>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <omp.h>


#define FMT_HEADER_ONLY
#include "fmt/format.h"

template <typename T>
T mode(const std::vector<T>& vec) {
    std::unordered_map<T, uint32_t> counts;
    for (const T& val : vec) {
        ++counts[val];
    }
    auto max_it = std::max_element(counts.begin(), counts.end(),
        [](const auto& a, const auto& b) { return a.second < b.second; });
    return max_it->first;
}

std::array<uint64_t, M>
get_sketch_mode(const std::vector<std::array<uint64_t, M>> input)
{
    const uint32_t sketches_len = input.size();
    std::array<uint64_t, M> res{0};
    std::vector<uint64_t> tmp(sketches_len);
    for (int n = 0; n < M; ++n) {
        for (int i = 0; i < sketches_len; ++i) {
            tmp[i] = input[i][n];
        }
        res[n] = mode(tmp);
    }
    return res;
}

std::bitset<M>
get_mode_bools(
    const std::array<uint64_t, M> modes,
    const std::array<uint64_t, M> input_hashes)
{
    std::bitset<M> res;
    for (int i = 0; i < M; ++i) {
        res[i] = (input_hashes[i] == modes[i]);
    }
    return res;
}

void
dist_mode(
    const std::vector<std::array<uint64_t, M>> sketches,
    const std::vector<std::string> files_names,
    const int num_of_threads,
    const bool edges)
{
    const size_t len_sketches = sketches.size();
    const std::array<uint64_t, M> mode = get_sketch_mode(sketches);
    std::vector<std::bitset<M>> bit_sketches(len_sketches);
    #pragma omp parallel for
    for (int i = 0; i < len_sketches; ++i) {
        bit_sketches[i] = get_mode_bools(mode, sketches[i]);
    }

    if (edges) {
        std::vector<std::stringstream> outputs(num_of_threads);
        #pragma omp parallel for
        for (int i = 0; i < len_sketches - 1; ++i) {
            const std::string fn_w_tab = files_names[i] + '\t';
            for(int j = i + 1; j < len_sketches; ++j) {
                int res = (bit_sketches[i] & bit_sketches[j]).count();
                const std::bitset<M> not_and_bitset = ~(bit_sketches[i] | bit_sketches[j]);
                for (size_t x = 0; x < not_and_bitset.size(); ++x) {
                        res += not_and_bitset[x] && (sketches[i][x] == sketches[j][x]);
                }
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
                int res = (bit_sketches[i] & bit_sketches[j]).count();
                const std::bitset<M> not_and_bitset = ~(bit_sketches[i] | bit_sketches[j]);
                for (size_t x = 0; x < not_and_bitset.size(); ++x) {
                        res += not_and_bitset[x] && (sketches[i][x] == sketches[j][x]);
                }
                fmt::format_to(std::back_inserter(outputs[omp_get_thread_num()]), "{:.6}\t", res / db_m);
            }
            outputs[omp_get_thread_num()].push_back('\n');
        }
        for (const auto &it: outputs)
            fmt::print(fmt::to_string(it));
    }
}