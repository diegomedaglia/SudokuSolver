#pragma once
#include <algorithm>
#include <ostream>

#include "Board.h"
#include "Common.h"

namespace Sudoku
{
/**
* @brief Checks if the coordinate provided is valid
* @param Dims The dimension size of the board
* @param coord coordinate 
* @throw std::out_of_range if the coordinate is greater than 8
*/
    void checkCoord( Num Dims, Num coord );

/**
* @brief Checks if the coordinates provided are valid
* @param Dims The dimension size of the board
* @param line coordinate line
* @param col coordinate column
* @throw std::out_of_range if any coordinate is greater than 8
*/
    void checkCoords( Num Dims, Num line, Num col );

/**
* @brief Checks if the value provided is valid
* @param Dims The dimension size of the board
* @param value The value to check
* @throw std::invalid_argument if the value is greater than 9
*/
    void checkValue( Num Dims, Num value );

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

std::ostream& operator<<( std::ostream& stream, const Sudoku::Board& board );
