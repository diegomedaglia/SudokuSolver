#include <stdexcept>
#include <algorithm>
#include <string>
#include "Board.h"

static void checkCoords( Num line, Num col );
static void checkCoord( Num coord );

Board::Board( const std::array<std::array<Num, 9>, 9>& values )
{
    for( int i = 0; i < DIMS; ++i )
    {
        for( int j = 0; j < DIMS; ++j )
        {
            auto& val = values[i][j];
            if( val != 0 )
            {
                m_board[i][j].setVal( val );
            }
            else
            {
                m_board[i][j] = Cell();
            }
        }
    }
    updatePossibleValues();
}

Cell Board::cell( int row, int col ) const
{
    checkCoords( row, col );
    return m_board[row][col];
}

void Board::updatePossibleValues() noexcept
{
    bool gotUpdate = false;
    do
    {
        gotUpdate = false;
        for( int i = 0; i < DIMS; ++i )
        {
            auto rowUpdated = updateInRow( i );
            auto colUpdated = updateInCol( i );
            auto qUpdated = updateInQuadrant( i );

            if( !gotUpdate )
            {
                gotUpdate = rowUpdated || colUpdated || qUpdated;
            }
        }
    }
    while( gotUpdate );
}

bool Board::updateInRow( Num row ) noexcept
{
    bool updatedOne = false;

    Nums existingNumbers;
    for( int i = 0; i < DIMS; ++i )
    {
        if( m_board[row][i].hasVal() )
        {
            existingNumbers.push_back( m_board[row][i].getVal() );
        }
    }
    for( int i = 0; i < DIMS; ++i )
    {
        if( !m_board[row][i].hasVal() )
        {
            m_board[row][i].remove( existingNumbers );
            if( m_board[row][i].hasVal() )
            {
                updatedOne = true;
            }
        }
    }

    return updatedOne;
}

bool Board::updateInCol( Num col ) noexcept
{
    Nums existingNumbers;
    bool updatedOne = false;

    for( int i = 0; i < DIMS; ++i )
    {
        if( m_board[i][col].hasVal() )
        {
            existingNumbers.push_back( m_board[i][col].getVal() );
        }
    }
    for( int i = 0; i < DIMS; ++i )
    {
        if( !m_board[i][col].hasVal() )
        {
            m_board[i][col].remove( existingNumbers );
            if( m_board[i][col].hasVal() )
            {
                updatedOne = true;
            }
        }
    }
    return updatedOne;
}
/*
    0              1            2

   0,0 0,1 0,2 | 0,3 0,4 0,5 | 0,6 0,7 0,8
0  1,0 1,1 1,2 | 1,3 1,4 1,5 | 1,6 1,7 1,8                 0 1 2
   2,0 2,1 2,2 | 2,3 2,4 2,5 | 2,6 2,7 2,8                 3 4 5
   ---------------------------------------                 6 7 8
   3,0 3,1 3,2 | 3,3 3,4 3,5 | 3,6 3,7 3,8
1  4,0 4,1 4,2 | 4,3 4,4 4,5 | 4,6 4,7 4,8
   5,0 5,1 5,2 | 5,3 5,4 5,5 | 5,6 5,7 5,8
   ---------------------------------------
   6,0 6,1 6,2 | 6,3 6,4 6,5 | 6,6 6,7 6,8
2  7,0 7,1 7,2 | 7,3 7,4 7,5 | 7,6 7,7 7,8
   8,0 8,1 8,2 | 8,3 8,4 8,5 | 8,6 8,7 8,8
*/

bool Board::updateInQuadrant( Num quadrant ) noexcept
{
    Nums existingNumbers;
    bool updatedOne = false;
    
    const int rowMult = ( quadrant / 3 );
    const int colMult = ( quadrant % 3 );

    const int startRow = rowMult * 3;
    const int startCol = colMult * 3;

    for( int i = startRow ; i < startRow + 3; ++i )
    {
        for( int j = startCol; j < startCol + 3; ++j )
        {
            if( m_board[i][j].hasVal() )
            {
                existingNumbers.push_back( m_board[i][j].getVal() );
            }
        }
    }
    for( int i = startRow; i < startRow + 3; ++i )
    {
        for( int j = startCol; j < startCol + 3; ++j )
        {
            if( !m_board[i][j].hasVal() )
            {
                m_board[i][j].remove( existingNumbers );
                if( m_board[i][j].hasVal() )
                {
                    updatedOne = true;
                }
            }
        }
    }
    return updatedOne;
}

Num Board::at( int row, int col ) const
{
    checkCoords( row, col );
    return m_board[row][col].getVal();
}

void Board::set( int row, int col, Num number )
{
    checkCoords( row, col );
    m_board[row][col].setVal( number );

    updatePossibleValues();
}

static void checkCoords( Num line, Num col )
{
    checkCoord( line );
    checkCoord( col );
}

static void checkCoord( Num coord )
{
    if( coord > 8 )
        throw std::out_of_range( "invalid coordinate: " + std::to_string( coord ) );
}

CoordCellList Board::sortedPossibilities()
{
    CoordCellList result;

    for( int i = 0; i < DIMS; ++i )
    {
        for( int j = 0; j < DIMS; ++j )
        {
            if( !m_board[i][j].hasVal() )
            {
                result.push_back( std::make_tuple( i, j, m_board[i][j] ) );
            }
        }
    }

    std::sort( result.begin(), result.end(), 
        []( const std::tuple<int, int, Cell>& lhs, const std::tuple<int, int, Cell>& rhs )
        { 
            const auto& cell1 = std::get<ccCELL>( lhs );
            const auto& cell2 = std::get<ccCELL>( rhs );

            auto cell1Possibilities = cell1.possibilities().size();
            auto cell2Possibilities = cell2.possibilities().size();

            return cell1Possibilities < cell2Possibilities;
        } );

    return result;
}
