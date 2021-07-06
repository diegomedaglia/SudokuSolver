#pragma once
#include <algorithm>
#include <string>
#include <stdexcept>
#include <array>
#include "Common.h"

namespace Sudoku
{
/**
* @brief Checks if the coordinate provided is valid
* @param coord coordinate 
* @throw std::out_of_range if the coordinate is greater than 8
*/
template<std::size_t Dims>
void checkCoord( Num coord )
{
    if( coord > Dims - 1 )
        throw std::out_of_range( "invalid coordinate: " + std::to_string( coord ) );
}

/**
* @brief Checks if the coordinates provided are valid
* @param line coordinate line
* @param col coordinate column
* @throw std::out_of_range if any coordinate is greater than 8
*/
template<std::size_t Dims>
void checkCoords( Num line, Num col )
{
    checkCoord<Dims>( line );
    checkCoord<Dims>( col );
}

/**
* @brief Checks if the value provided is valid
* @param value The value to check
* @throw std::invalid_argument if the value is greater than 9
*/
template<std::size_t Dims>
void checkValue( Num value )
{
    if( value > Dims )
        throw std::invalid_argument( "invalid value: " + std::to_string( value ) );
}

/**
* @brief Checks if a vector of Num contains a value
* @param nums the vector to perform the check on
* @param val The value to find in nums
* @return True if val was found in nums, false otherwise.
*/
inline bool contains( const Nums& nums, Num val ) noexcept
{
    return std::find( nums.begin(), nums.end(), val ) != nums.end();
}

}
