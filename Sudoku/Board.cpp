#include <stdexcept>
#include <algorithm>
#include <string>
#include <sstream>

#include "Board.h"
#include "Utils.h"

using namespace Sudoku;

Board::Board()
{
    performInCells( 
        [this]( auto i, auto j, Cell& cell ) 
        { 
            return true; 
        } );
}

Board::Board( const std::array<std::array<Num, 9>, 9>& values )
{
    performInCells(
        [this, &values]( auto i, auto j, Cell& cell )
        {
            auto& val = values[i][j];
            checkValue( val );

            if( val != 0 )
            {
                cell.setVal( val );
            }
            return true;
        }
    );

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
            gotUpdate |= updateInRow( i );
            gotUpdate |= updateInCol( i );
            gotUpdate |= updateInQuadrant( i );

            for( int j = 0; j < i; ++j )
            {
                gotUpdate |= updateGroup( getRowCells( j ) );
                gotUpdate |= updateGroup( getColCells( j ) );
                gotUpdate |= updateGroup( getQuadrantCells( j ) );
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
        auto& cell = m_board[row][i];
        if( !cell.hasVal() )
        {
            updatedOne |= cell.remove( existingNumbers );
        }
    }

    return updatedOne;
}

bool Board::updateGroup( const std::vector<Cell*>& group ) noexcept
{
    bool updatedOne = false;

    // find 2, 3, and 4 subgroups of cells in the provided group
    // with the same possibilities. E.g. two cells with possibilities = { 1, 2},
    // three cells with possibilities = {1, 2, 4}.
    for( int i = 2; i < 5; ++i )
    {
        std::vector<Cell*> cellsWithSamePossibilities;
        for( auto cell : group )
        {
            if( cell->possibilities().size() == i )
            {
                cellsWithSamePossibilities.push_back( cell );
            }
        }
        if( cellsWithSamePossibilities.size() == i )
        {
            bool allEqual = std::all_of( ++( cellsWithSamePossibilities.begin() ), cellsWithSamePossibilities.end(),
                [&cellsWithSamePossibilities]( auto cell )
                {
                    return cell->possibilities() == cellsWithSamePossibilities.front()->possibilities();
                }
            );

            if( allEqual )
            {
                auto possibilitiesToRemove = cellsWithSamePossibilities.front()->possibilities();
                for( auto cell : group )
                {
                    // if the cellsWithSamePossibilities vector does not contain 'cell'
                    if( std::find( cellsWithSamePossibilities.begin(), cellsWithSamePossibilities.end(), cell ) == cellsWithSamePossibilities.end() )
                    {
                        updatedOne |= cell->remove( possibilitiesToRemove );
                    }
                }
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
            updatedOne |= m_board[i][col].remove( existingNumbers );
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
        [this, &existingNumbers]( auto i, auto j, auto& cell ) 
        {
            if( cell.hasVal() )
            {
                existingNumbers.push_back( cell.getVal() );
            }
            return true;
        } );

    performInQuadrant( quadrant, 
        [this, &existingNumbers, &updatedOne]( auto i, auto j, auto& cell )
        {
            if( !cell.hasVal() )
            {
                updatedOne |= cell.remove( existingNumbers );
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
        [&result]( auto i, auto j, auto& cell )
        {
            if( !cell.hasVal() )
            {
                result.push_back( { i, j, cell.possibilities() } );
            }
            return true;
        } );

    std::sort( result.begin(), result.end(), 
        []( const CoordPossibilities& lhs, const CoordPossibilities& rhs )
        { 
            return lhs.possibilities.size() < rhs.possibilities.size();
        } );

    return result;
}

bool Board::isValid()
{
    bool result = true;

    performInCells(
        [&result, this]( auto i, auto j, auto& cell )
        {
            if( cell.possibilities().empty() )
            {
                m_offendingVal = std::make_tuple( i, j, 0, 0, 0 );
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
    for( int i = 0; i < DIMS; ++i )
    {
        if( !validateQuadrant( i ) )
        {
            result = false;
            break;
        }
    }

    return result;
}

bool Board::isSolved() const noexcept
{
    bool result = true;
    performInCells( 
        [&result]( auto i, auto j, auto& cell )
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
    for( int i = 0; i < DIMS; ++i )
    {
        for( int j = 0; j < DIMS; ++j )
        {
            if( at( i, j ) != rhs.at( i, j ) )
                return false;
        }
    }

    return true;
}

bool Board::operator!=( const Board& rhs ) const
{
    return !operator==( rhs );
}

bool Board::validateQuadrant( int quadrant)
{
    bool found = false;
    std::map<Num, std::pair<int, int>> values;
    performInQuadrant( quadrant,
        [this, &found, &values]( auto k, auto l, auto& cell )
        {
            if( cell.hasVal() )
            {
                auto val = cell.getVal();
                if( values.find( val ) != values.end() )
                {
                    m_offendingVal = std::make_tuple( k, l, std::get<0>( values[val] ), std::get<1>( values[val] ), val );
                    found = true;
                }
                else
                {
                    values[val] = std::make_pair( k, l );
                }
            }
            return !found;
        } );

    return !found;
}

void Board::performInCells( std::function<bool( const int, const int, Cell& )> func )
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

void Board::performInCells( std::function<bool( const int, const int, const Cell& )> func ) const
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

void Board::performInQuadrant( Num quadrant, std::function<bool( const int row, const int col, Cell& cell )> func )
{
    const int rowMult = ( quadrant / 3 );
    const int colMult = ( quadrant % 3 );

    const int startRow = rowMult * 3;
    const int startCol = colMult * 3;

    for( int i = startRow; i < startRow + 3; ++i )
    {
        for( int j = startCol; j < startCol + 3; ++j )
        {
            if( !func( i, j, m_board[i][j] ) )
                return;
        }
    }
}

std::vector<Cell*> Board::getRowCells( int row )
{
    std::vector<Cell*> result;

    for( int i = 0; i < DIMS; ++i )
    {
        Cell* cellPtr = m_board[row].data() + i;
        result.push_back( cellPtr );
    }

    return result;
}

std::vector<Cell*> Board::getColCells( int col )
{
    std::vector<Cell*> result;

    for( int i = 0; i < DIMS; ++i )
    {
        Cell* cellPtr = m_board[i].data() + col;
        result.push_back( cellPtr );
    }
    return result;
}

std::vector<Cell*> Board::getQuadrantCells( int quadrant )
{
    std::vector<Cell*> result;

    performInQuadrant( quadrant, [&result, this]( auto i, auto j, Cell& cell )
        {
            Cell* cellPtr = m_board[i].data() + j;
            result.push_back( cellPtr );
            return true;
        } );
    return result;
}

/**
* Calculates the hash of the board.
* @param b the board to calculate the hash for
* @return the board's hash.
*/

std::size_t BoardHasher::operator()( Board const& b ) const noexcept
{
    size_t seed = 0;

    std::array<Num, 9> leftovers;

    for( int i = 0; i < DIMS; ++i )
    {
        std::array<Num, 8> rowvals;
        for( int j = 0; j < 8; ++j )
        {
            rowvals[j] = b.at( i, j );
        }

        leftovers[i] = b.at( i, 8 );

        const auto u64hash = u64Hasher( *( reinterpret_cast< const std::uint64_t* >( rowvals.data() ) ) );
        combineHash( seed, u64hash );
    }
    const auto u64hash = u64Hasher( *( reinterpret_cast< const std::uint64_t* >( leftovers.data() ) ) );
    combineHash( seed, u64hash );

    const auto u8hash = u8Hasher( leftovers[8] );
    combineHash( seed, u8hash );

    return seed;
}

/**
* boost::hash_combine
*/

void BoardHasher::combineHash( size_t& seed, const size_t& hash ) noexcept
{
    seed ^= hash + 0x9e3779b9 + ( seed << 6 ) + ( seed >> 2 );
}

std::ostream& operator<<( std::ostream& stream, const Board& board )
{
    for( int i = 0; i < DIMS; ++i )
    {
        for( int j = 0; j < DIMS; ++j )
        {
            stream << std::to_string( board.at( i, j ) ) << " ";
        }
        stream << std::endl;
    }
    return stream;
}
