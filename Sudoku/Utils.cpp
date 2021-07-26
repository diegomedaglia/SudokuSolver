#include "Utils.h"
#include <string>
#include <stdexcept>
#include <iomanip>

void Sudoku::checkCoord( Num Dims, Num coord )
{
    if( coord > Dims - 1 )
        throw std::out_of_range( "invalid coordinate: " + std::to_string( coord ) );
}

void Sudoku::checkCoords( Num Dims, Num line, Num col )
{
    checkCoord( Dims, line );
    checkCoord( Dims, col );
}

void Sudoku::checkValue( Num Dims, Num value )
{
    if( value > Dims )
        throw std::invalid_argument( "invalid value: " + std::to_string( value ) );
}

std::ostream& operator<<( std::ostream& stream, const Sudoku::Board& board )
{
    auto width = board.dimension() / 9;
    if( board.dimension() % 9 )
    {
        width++;
    }

    for( Sudoku::Num i = 0; i < board.dimension(); ++i )
    {
        for( Sudoku::Num j = 0; j < board.dimension(); ++j )
        {
            stream << std::setw( width ) << std::dec << std::to_string( board.at( i, j ) ) << " ";
        }
        stream << std::endl;
    }
    return stream;
}
