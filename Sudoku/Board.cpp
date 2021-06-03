#include <stdexcept>
#include <algorithm>
#include <string>
#include <sstream>

#include "Board.h"
#include "Utils.h"

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

    if( !isValid() )
        throw std::invalid_argument( "board has invalid values" );

    updatePossibleValues();
}

Board::Board( const Board& other )
{
    *this = other;
}

Board& Board::operator=( const Board& other )
{
    this->m_board = other.m_board;
    return *this;
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

    performInQuadrant( quadrant, 
        [this, &existingNumbers]( int i, int j ) -> bool
        {
            if( m_board[i][j].hasVal() )
            {
                existingNumbers.push_back( m_board[i][j].getVal() );
            }
            return true;
        } );

    performInQuadrant( quadrant, 
        [this, &existingNumbers, &updatedOne]( int i, int j ) -> bool
        {
            if( !m_board[i][j].hasVal() )
            {
                m_board[i][j].remove( existingNumbers );
                if( m_board[i][j].hasVal() )
                {
                    updatedOne = true;
                }
            }
            return true;
        } );
    
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

CoordPossibilitiesList Board::sortedPossibilities()
{
    CoordPossibilitiesList result;

    performInCells(
        [&result]( int i, int j, Cell& cell )
        {
            if( !cell.hasVal() )
            {
                result.push_back( std::make_tuple( i, j, cell.possibilities() ) );
            }
            return true;
        } );

    std::sort( result.begin(), result.end(), 
        []( const CoordPossibilities& lhs, const CoordPossibilities& rhs )
        { 
            const auto& poss1 = std::get<ccPOSSIBILITIES>( lhs );
            const auto& poss2 = std::get<ccPOSSIBILITIES>( rhs );

            auto poss1Size = poss1.size();
            auto poss2Size = poss2.size();

            return poss1Size < poss2Size;
        } );

    return result;
}

bool Board::isValid()
{
    bool result = true;

    performInCells(
        [&result, this]( auto i, auto j, auto cell )
        {
            if( cell.possibilities().empty() )
            {
                m_offendingVal = std::make_tuple( i, j, 0, 0, 0 );
                result = false;
                return false;
            }

            if( !validateInQuadrant( i, j ) )
            {
                result = false;
                return false;
            }


            if( cell.hasVal() )
            {
                auto val = cell.getVal();
                // search in row and column for same value
                for( int k = 0; k < DIMS; ++k )
                {
                    if( k != i )
                    {
                        auto& cell2 = m_board[k][j];
                        if( cell2.hasVal() && val == cell2.getVal() )
                        {
                            m_offendingVal = std::make_tuple( i, j, k, j, val );
                            result = false;
                            break;
                        }
                    }
                    if( k != j )
                    {
                        auto& cell2 = m_board[i][k];
                        if( cell2.hasVal() && val == cell2.getVal() )
                        {
                            m_offendingVal = std::make_tuple( i, j, i, k, val );
                            result = false;
                            break;
                        }
                    }
                }
            }
            return result;
        }
    );

    return result;
}

bool Board::isSolved()
{
    bool result = true;
    performInCells( 
        [&result]( auto i, auto j, auto cell )
        { 
            if( !cell.hasVal() )
            {
                result = false;
                return false;
            }
            return true;
        } );

    return result;
}

bool Board::operator==( const Board& rhs ) const
{
    bool result = true;

    performInCells(
        [&result, &rhs](int i, int j, const Cell& cell )
        {
            if( cell.getVal() != rhs.at( i, j ) )
            {
                result = false;
                return false;
            }
            return true;
        }
    );

    return result;
}

bool Board::operator!=( const Board& rhs ) const
{
    return !operator==( rhs );
}

bool Board::validateInQuadrant( Num row, Num col )
{
    checkCoords( row, col );

    auto val = m_board[row][col].getVal();
    if( val == 0 )
        return true;

    bool found = false;
    performInQuadrant( getQuadrant( row, col ),
        [this, val, row, col, &found]( int k, int l )
        {
            if( row == k && col == l )
                return true;

            if( m_board[k][l].hasVal() && m_board[k][l].getVal() == val )
            {
                m_offendingVal = std::make_tuple( row, col, k, l, val );
                found = true;
            }
            return !found;
        } );

    return !found;
}

void Board::performInCells( std::function<bool( int, int, Cell& )> func )
{
    for( int i = 0; i < DIMS; ++i )
    {
        for( int j = 0; j < DIMS; ++j )
        {
            if( !func( i, j, m_board[i][j] ) )
                return;
        }
    }
}

void Board::performInCells( std::function<bool( int, int, const Cell& )> func ) const
{
    for( int i = 0; i < DIMS; ++i )
    {
        for( int j = 0; j < DIMS; ++j )
        {
            if( !func( i, j, m_board[i][j] ) )
                return;
        }
    }
}

void Board::performInQuadrant( Num quadrant, std::function<bool( int row, int col )> func )
{
    const int rowMult = ( quadrant / 3 );
    const int colMult = ( quadrant % 3 );

    const int startRow = rowMult * 3;
    const int startCol = colMult * 3;

    for( int i = startRow; i < startRow + 3; ++i )
    {
        for( int j = startCol; j < startCol + 3; ++j )
        {
            if( !func( i, j ) )
                return;
        }
    }
}
