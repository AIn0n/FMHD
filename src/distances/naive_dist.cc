#include "distances/naive_dist.hpp"
#include "commons.hpp"

#include <omp.h>
#include <iostream>
#include <sstream>


#define FMT_HEADER_ONLY
#include "fmt/format.h"

void
dist_naive(
    const std::vector<std::array<uint64_t, M>> sketches,
    const std::vector<std::string> files_names,
    const int num_of_threads,
    const bool edges)
{
    const size_t len_sketches = sketches.size();

    if (edges) {
        std::vector<std::stringstream> outputs(num_of_threads);
        #pragma omp parallel for
        for (int i = 0; i < len_sketches - 1; ++i) {
            const std::string fn_w_tab = files_names[i] + '\t';
            for(int j = i + 1; j < len_sketches; ++j) {
                int sum = 0;
                for (size_t x = 0; x < M; ++x) {
                    sum += (sketches[i][x] == sketches[j][x]);
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
                int sum = 0;
                for (size_t x = 0; x < M; ++x) {
                    sum += (sketches[i][x] == sketches[j][x]);
                }
                fmt::format_to(std::back_inserter(outputs[omp_get_thread_num()]), "{:.6}\t", sum / db_m);
            }
            outputs[omp_get_thread_num()].push_back('\n');
        }
        for (const auto &it: outputs)
            fmt::print(fmt::to_string(it));
    }
}