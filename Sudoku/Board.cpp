#include <stdexcept>

#include "Board.h"
#include "Utils.h"

using Sudoku::Board;
using Sudoku::Num;
using Sudoku::Cell;
using Sudoku::CoordPossibilitiesList;

Board::Board( Num dims ) :
    m_blockSide( dims ),
    m_dimension( m_blockSide* m_blockSide ),
    m_board( m_dimension, Cells( m_dimension, Cell( m_dimension ) ) )
{
}


Board::Board( Num dims, const Board::InputArray& values ) :
    m_blockSide( dims ),
    m_dimension( m_blockSide* m_blockSide ),
    m_board( m_dimension, Cells( m_dimension, Cell( m_dimension ) ) )
{
    performInCells(
        [this, &values]( auto i, auto j, Cell& cell )
        {
            auto& val = values[i][j];
            checkValue( m_dimension, val );

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
    this->m_blockSide = other.m_blockSide;
    this->m_dimension = other.m_dimension;
    return *this;
}

Num Board::at( Num row, Num col ) const
{
    checkCoords( m_dimension, row, col );
    return m_board[row][col].getVal();
}


Cell Board::cell( Num row, Num col ) const
{
    checkCoords( m_dimension, row, col );
    return m_board[row][col];
}


void Board::set( Num row, Num col, Num number )
{
    checkCoords( m_dimension, row, col );
    m_board[row][col].setVal( number );

    updatePossibleValues();
}


CoordPossibilitiesList Board::sortedPossibilities()
{
    CoordPossibilitiesList result;

    performInCells(
        [&result]( auto i, auto j, Cell& cell )
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
                m_offendingVal = std::make_tuple( i, j, ( Num )0, ( Num )0, ( Num )0 );
                result = false;
                return false;
            }

            if( cell.hasVal() )
            {
                auto val = cell.getVal();
                // search in row and column for same value
                for( Num k = 0; k < m_dimension; ++k )
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
    for( Num i = 0; i < m_dimension; ++i )
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
        [&result]( auto, auto, auto& cell )
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


std::vector<Cell*> Board::getRowCells( Num row )
{
    std::vector<Cell*> result;

    for( Num i = 0; i < m_dimension; ++i )
    {
        Cell* cellPtr = m_board[row].data() + i;
        result.push_back( cellPtr );
    }

    return result;
}


std::vector<Cell*> Board::getColCells( Num col )
{
    std::vector<Cell*> result;

    for( Num i = 0; i < m_dimension; ++i )
    {
        Cell* cellPtr = m_board[i].data() + col;
        result.push_back( cellPtr );
    }
    return result;
}


std::vector<Cell*> Board::getQuadrantCells( Num quadrant )
{
    std::vector<Cell*> result;

    performInQuadrant( quadrant, [&result, this]( auto i, auto j, Cell& )
        {
            Cell* cellPtr = m_board[i].data() + j;
            result.push_back( cellPtr );
            return true;
        } );
    return result;
}


bool Board::operator==( const Board& rhs ) const
{
    for( Num i = 0; i < m_dimension; ++i )
    {
        for( Num j = 0; j < m_dimension; ++j )
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


void Board::performInCells( std::function<bool( const Num, const Num, Cell& )> func )
{
    for( Num i = 0; i < m_dimension; ++i )
    {
        for( Num j = 0; j < m_dimension; ++j )
        {
            if( !func( i, j, m_board[i][j] ) )
                return;
        }
    }
}

void Board::performInCells( std::function<bool( const Num, const Num, const Cell& )> func ) const
{
    for( Num i = 0; i < m_dimension; ++i )
    {
        for( Num j = 0; j < m_dimension; ++j )
        {
            if( !func( i, j, m_board[i][j] ) )
                return;
        }
    }
}

void Board::performInQuadrant( Num quadrant, std::function<bool( const Num row, const Num col, Cell& )> func )
{
    const Num rowMult = quadrant / m_blockSide;
    const Num colMult = quadrant % m_blockSide;

    const Num startRow = rowMult * m_blockSide;
    const Num startCol = colMult * m_blockSide;

    for( Num i = startRow; i < startRow + m_blockSide; ++i )
    {
        for( Num j = startCol; j < startCol + m_blockSide; ++j )
        {
            if( !func( i, j, m_board[i][j] ) )
                return;
        }
    }
}

bool Board::validateQuadrant( Num quadrant )
{
    bool found = false;
    std::map<Num, std::pair<Num, Num>> values;
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

void Board::updatePossibleValues() noexcept
{
    bool gotUpdate = false;
    do
    {
        gotUpdate = false;
        for( Num i = 0; i < m_dimension; ++i )
        {
            gotUpdate |= updateInRow( i );
            gotUpdate |= updateInCol( i );
            gotUpdate |= updateInQuadrant( i );

            for( Num j = 0; j < i; ++j )
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
    for( Num i = 0; i < m_dimension; ++i )
    {
        if( m_board[row][i].hasVal() )
        {
            existingNumbers.push_back( m_board[row][i].getVal() );
        }
    }

    for( Num i = 0; i < m_dimension; ++i )
    {
        auto& cell = m_board[row][i];
        if( !cell.hasVal() )
        {
            updatedOne |= cell.remove( existingNumbers );
        }
    }

    return updatedOne;
}

bool Board::updateInCol( Num col ) noexcept
{
    Nums existingNumbers;
    bool updatedOne = false;

    for( Num i = 0; i < m_dimension; ++i )
    {
        if( m_board[i][col].hasVal() )
        {
            existingNumbers.push_back( m_board[i][col].getVal() );
        }
    }
    for( Num i = 0; i < m_dimension; ++i )
    {
        if( !m_board[i][col].hasVal() )
        {
            updatedOne |= m_board[i][col].remove( existingNumbers );
        }
    }

    return updatedOne;
}


bool Board::updateInQuadrant( Num quadrant ) noexcept
{
    Nums existingNumbers;
    bool updatedOne = false;

    performInQuadrant( quadrant,
        [this, &existingNumbers]( auto, auto, auto& cell )
        {
            if( cell.hasVal() )
            {
                existingNumbers.push_back( cell.getVal() );
            }
            return true;
        } );

    performInQuadrant( quadrant,
        [this, &existingNumbers, &updatedOne]( auto, auto, auto& cell )
        {
            if( !cell.hasVal() )
            {
                updatedOne |= cell.remove( existingNumbers );
            }
            return true;
        } );

    return updatedOne;
}

bool Board::updateGroup( const std::vector<Cell*>& group ) noexcept
{
    bool updatedOne = false;

    // find 2, 3, and 4 subgroups of cells in the provided group
    // with the same possibilities. E.g. two cells with possibilities = { 1, 2},
    // three cells with possibilities = {1, 2, 4}.
    for( Num i = 2; i < m_dimension / 2; ++i )
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
