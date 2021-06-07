#include <string>
#include <stdexcept>
#include <array>

#include "Utils.h"

void Sudoku::checkCoords( Num line, Num col )
{
    checkCoord( line );
    checkCoord( col );
}

void Sudoku::checkCoord( Num coord )
{
    if( coord > 8 )
        throw std::out_of_range( "invalid coordinate: " + std::to_string( coord ) );
}

void Sudoku::checkValue( Num value )
{
    if ( value > 9 )
        throw std::invalid_argument( "invalid value: " + std::to_string( value ) );
}
