#include "Utils.h"
#include <string>
#include <stdexcept>
#include <array>

void checkCoords( Num line, Num col )
{
    checkCoord( line );
    checkCoord( col );
}

void checkCoord( Num coord )
{
    if( coord > 8 )
        throw std::out_of_range( "invalid coordinate: " + std::to_string( coord ) );
}

void checkValue( Num value )
{
    if ( value > 9 )
        throw std::invalid_argument( "invalid value: " + std::to_string( value ) );
}

int getQuadrant( Num row, Num col )
{
    static constexpr std::array<std::array<Num, 3>, 3> quadrantNumbers{
    {
        { 0, 1, 2 },
        { 3, 4, 5 },
        { 6, 7, 8 },
    } };

    checkCoords( row, col );

    return quadrantNumbers[row / 3][col / 3];
}
