#include <iostream>
#include <fstream>
#include <array>
#include <stdexcept>

#include "FileParser.h"

Sudoku::Board Sudoku::parseFile( Num BlockSize, const std::string& filename )
{
    std::ifstream filestream{ filename };
    if( !filestream.is_open() )
    {
        throw std::invalid_argument( "Can't open file " + filename );
    }

    auto Dims = BlockSize * BlockSize;

    Board::InputArray values( Dims, Nums( Dims ) );

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

    if( i < Dims )
    {
        throw std::runtime_error( "file " + filename + " had incomplete board." );
    }

    return { BlockSize, values };
}
