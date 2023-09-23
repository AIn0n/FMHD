#include "sketch.hpp"

#include <limits> /* max number which we can put into uint64_t */
#include "xxhash64.hpp" /* hashing function */
#include <unordered_map>

inline uint64_t hash(const std::string_view input)
{
    return XXHash64::hash(input.data(), input.length(), SEED);
}

std::array<char, 128>
init_rc_table(void)
{
    std::array<char, 128> res = {0};
    res['A'] = 'T';
    res['T'] = 'A';
    res['U'] = 'A';
    res['C'] = 'G';
    res['G'] = 'C';
    res['M'] = 'K';
    res['K'] = 'M';
    res['R'] = 'Y';
    res['Y'] = 'R';
    res['W'] = 'W';
    res['S'] = 'S';
    res['V'] = 'B';
    res['B'] = 'V';
    res['H'] = 'D';
    res['D'] = 'H';
    res['N'] = 'N';

    return res;
}

std::string
get_reversed_complement(
    const std::string& input,
    const std::array<char, 128>& rc_lookup)
{
    std::string output = std::string(input.size(), 'N');
    for (int i = 0; i < input.size(); ++i)
        output[input.size() - i - 1] = rc_lookup[input[i]];
    return output;
}

std::array<uint64_t, M>
get_sketch(
    const std::string input,
    const size_t k,
    const std::array<char, 128>& rc_lookup)
{
    std::array<uint64_t, M> hashes{};
    hashes.fill(std::numeric_limits<uint64_t>::max());
    const std::string input_rc = get_reversed_complement(input, rc_lookup);
    const auto size = input.size();
    for (size_t i = 0, j = size - k; i < size - k + 1; ++i, --j) {
        const std::string_view kmer(input.data() + i, k);
        const std::string_view rc_kmer(input_rc.data() + j, k);
        const uint64_t canonical = hash((kmer <= rc_kmer) ? kmer : rc_kmer);
        const uint64_t where = canonical & MASK;
        hashes[where] = std::min(hashes[where], canonical);
    }
    return hashes;
}
