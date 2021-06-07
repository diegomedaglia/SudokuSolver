#include <fstream>
#include <array>
#include <stdexcept>

#include "FileParser.h"

Board parseFile( const std::string& filename )
{
    std::ifstream filestream{ filename };
    if( !filestream.is_open() )
    {
        throw std::invalid_argument( "Can't open file " + filename );
    }

    std::array<std::array<Num, DIMS>, DIMS> values;

    int i = 0;
    int j = 0;

    while( i < DIMS && !filestream.fail() )
    {
        int n;
        filestream >> std::dec >> n;
        values[i][j++] = static_cast< Num >( n );

        if( j == DIMS )
        {
            ++i;
            j = 0;
        }
    }

    if( filestream.fail() )
    {
        throw std::runtime_error( "Error parsing file: " + filename );
    }

    if( i < DIMS )
    {
        throw std::runtime_error( "file " + filename + " had incomplete board." );
    }

    return { values };
}