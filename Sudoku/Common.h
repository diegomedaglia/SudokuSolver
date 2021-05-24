#pragma once
#include <cstdint>
#include <vector>

using Num = std::uint8_t;
using Nums = std::vector<Num>;

constexpr int DIMS = 9;

inline bool contains( const Nums& nums, Num val )
{
    return std::find( nums.begin(), nums.end(), val ) != nums.end();
}

