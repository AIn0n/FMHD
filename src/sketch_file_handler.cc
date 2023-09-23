#include "sketch_file_handler.hpp"

#include <iostream>
#include <fstream>

void
save_sketches(
    const std::vector<std::array<uint64_t, M>> sketches,
    const std::vector<std::string> files_names,
    const std::string out_filename)
{
    std::ofstream file(out_filename, std::ios::binary);
    if (file.is_open()) {
        const size_t len_sketches = sketches.size();
        file.write(reinterpret_cast<const char*>(&len_sketches), sizeof(size_t));

        for (int i = 0; i < len_sketches; ++i) {
            file.write(reinterpret_cast<const char*>(sketches[i].data()), M * sizeof(uint64_t));

            const size_t name_size = files_names[i].size();
            file.write(reinterpret_cast<const char*>(&name_size), sizeof(size_t));
            file.write(files_names[i].c_str(), name_size);
        }
        file.close();
    }
}

bool
read_sketches(
    std::vector<std::array<uint64_t, M>>& sketches,
    std::vector<std::string>& names,
    const std::string input_file)
{
    std::ifstream file(input_file, std::ios::binary);
    if(file.is_open()) {
        size_t in_size, name_size;
        file.read(reinterpret_cast<char*>(&in_size), sizeof(size_t));
        sketches.resize(in_size);
        names.resize(in_size);

        for (size_t i = 0; i < in_size; ++i) {
            file.read(reinterpret_cast<char*>(sketches[i].data()), M * sizeof(uint64_t));
            file.read(reinterpret_cast<char*>(&name_size), sizeof(size_t));
            std::string name(name_size, ' ');
            file.read(&name[0], name_size);
            names[i] = name;
        }
        file.close();
        return true;
    }
    return false;
}