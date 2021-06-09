#pragma once

#include <iostream>
#include <string>
#include "Board.h"
#include <fstream>
#include <array>
#include <stdexcept>

namespace Sudoku
{

/**
* @brief Parses a file containing a representation of a board. Cells are separated
* by whitespace and have values in the range [0, 9]. 0 denotes empty cells.
* @param filename the path to the file
* @return A board with the values specified in the file
* @throw std::invalid_argument The filename can't be opened for reading
* @throw std::runtime_error An error occurred during parsing of the file.
*/
template<class BoardType>
BoardType parseFile( const std::string& filename )
{
    std::ifstream filestream{ filename };
    if( !filestream.is_open() )
    {
        throw std::invalid_argument( "Can't open file " + filename );
    }

    constexpr auto Dims = BoardType::Dimension;

    std::array<std::array<Num, Dims>, Dims> values;

    int i = 0;
    int j = 0;

    while( i < Dims && !filestream.fail() )
    {
        int n;
        filestream >> std::dec >> n;
        values[i][j++] = static_cast< Num >( n );

        if( j == Dims )
        {
            ++i;
            j = 0;
        }
    }

    if( filestream.fail() )
    {
        throw std::runtime_error( "Error parsing file: " + filename );
    }

    if( i < BoardType::Dimension )
    {
        throw std::runtime_error( "file " + filename + " had incomplete board." );
    }

    return { values };
}

}