#pragma once

#ifndef __CONSTANTS_H_
#define __CONSTANTS_H_

#include <cstdint>
#include <string>

constexpr uint64_t M = 1024; /* num of hashing functions */
constexpr float db_m = static_cast<float>(M);
const uint64_t SEED = 4390857934875L;
const std::string suffix = '/' + std::to_string(M) + '\n';

#endif