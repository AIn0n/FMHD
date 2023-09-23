#include "commons.hpp"

#include <iostream>

void
print_names(const std::vector<std::string> &names, const int len)
{
    std::cout << "##Names ";
    for (int i = len - 1; i > -1;--i)
        std::cout << names[i] << '\t';
    std::cout << '\n';
}