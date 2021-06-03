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

    validateBoard();

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

    performInQuadrant( quadrant, 
        [this, &existingNumbers]( int i, int j )
        {
            if( m_board[i][j].hasVal() )
            {
                existingNumbers.push_back( m_board[i][j].getVal() );
            }
        } );

    performInQuadrant( quadrant, 
        [this, &existingNumbers, &updatedOne]( int i, int j )
        {
            if( !m_board[i][j].hasVal() )
            {
                m_board[i][j].remove( existingNumbers );
                if( m_board[i][j].hasVal() )
                {
                    updatedOne = true;
                }
            }
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

void Board::validateBoard()
{
    for( int i = 0; i < DIMS; ++i )
    {
        for( int j = 0; j < DIMS; ++j )
        {
            validateInQuadrant( i, j );

            if( m_board[i][j].hasVal() )
            {
                auto val = m_board[i][j].getVal();
                // search in row and column for same value
                for( int k = 0; k < DIMS; ++k )
                {
                    if( k != i )
                    {
                        auto cell = m_board[k][j];
                        if( cell.hasVal() && val == cell.getVal() )
                            throw( std::invalid_argument( validationErrorBuilder( i, j, k, j ) ) );
                    }
                    if( k != j )
                    {
                        auto cell = m_board[i][k];
                        if( cell.hasVal() && val == cell.getVal() )
                            throw( std::invalid_argument( validationErrorBuilder( i, j, i, k ) ) );
                    }
                }
            }
        }
    }
}

void Board::validateInQuadrant( Num row, Num col )
{
    auto val = m_board[row][col].getVal();
    if( val == 0 )
        return;

    bool found = false;
    int offendingRow = 0;
    int offendingCol = 0;
    performInQuadrant( getQuadrant( row, col ),
        [this, val, row, col, &found, &offendingRow, &offendingCol]( int k, int l )
        {
            if( row == k && col == l )
                return;
            if( m_board[k][l].hasVal() && m_board[k][l].getVal() == val )
            {
                found = true;
                offendingRow = k;
                offendingCol = l;
            }
        } );
    if( found )
        throw( std::invalid_argument( validationErrorBuilder( row, col, offendingRow, offendingCol ) ) );
}

std::string Board::validationErrorBuilder( int row, int col, int row2, int col2 )
{
    std::stringstream ss;
    ss << "cell in (" << ( row + 1 ) << "," << ( col + 1 ) << ") has same value as ("
        << ( row2 + 1 ) << "," << ( col2 + 1 ) << ")";
    return ss.str();
}

void Board::performInQuadrant( Num quadrant, std::function<void( int row, int col )> func )
{
    const int rowMult = ( quadrant / 3 );
    const int colMult = ( quadrant % 3 );

    const int startRow = rowMult * 3;
    const int startCol = colMult * 3;

    for( int i = startRow; i < startRow + 3; ++i )
    {
        for( int j = startCol; j < startCol + 3; ++j )
        {
            func( i, j );
        }
    }
}
