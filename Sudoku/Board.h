#pragma once
#include <ostream>
#include <array>
#include <utility>
#include <functional>
#include <map>

#include "Common.h"
#include "Cell.h"
#include "Utils.h"

namespace Sudoku
{

/**
* @brief Structure used to store a board coordinate and a list
* of possible values for that Cell.
*/
struct CoordPossibilities
{
    Num row;
    Num col;
    Nums possibilities;
};

using CoordPossibilitiesList = std::vector<CoordPossibilities>;

/**
* @brief Represents the Sudoku board and provides operations to manipulate its values.
* @tparam BlockSide The side of the subregions of this board: the board consists of
* BlockSide^2 x BlockSide^2 grids subdivided into BlockSide x BlockSide blocks.
*/
template<Num BlockSide>
class Board
{
public:
    static constexpr Num Dimension = ( BlockSide * BlockSide );
private:
    using Cell = Cell<Dimension>;
public:
    /**
    * @brief Contructs an empty board (i.e. all cells unassigned).
    */
    Board() = default;
    /**
    * @brief Contructs a board with the values provided.
    */
    Board( const std::array<std::array<Num, Dimension>, Dimension>& values )
    {
        performInCells(
            [this, &values]( auto i, auto j, Cell& cell )
            {
                auto& val = values[i][j];
                checkValue<Dimension>( val );

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
    /**
    * @brief Copy constructor
    */
    Board( const Board& other )
    {
        *this = other;
    }
    /**
    * @brief Copy assignment
    */
    Board& operator=( const Board& other )
    {
        this->m_board = other.m_board;
        return *this;
    }
    /**
    * @brief Returns the assigned value for the specified cell. 0 is returned
    * for cells with no assigned value.
    *
    * @param row the cell row
    * @param col the cell column
    * @return The cell's value
    * @throw std::out_of_range if either coordinates are out of bounds
    */
    Num at( Num row, Num col ) const
    {
        checkCoords<Dimension>( row, col );
        return m_board[row][col].getVal();
    }
    /**
    * @brief Returns a copy of the cell at the specified location.
    *
    * @param row the cell row
    * @param col the cell column
    * @return A copy of the cell instance
    * @throw std::out_of_range if either coordinates are out of bounds
    */
    Cell cell( Num row, Num col ) const
    {
        checkCoords<Dimension>( row, col );
        return m_board[row][col];
    }
    /**
    * @brief Assigns a value to the cell at the specified location.
    *
    * @param row the cell row
    * @param col the cell column
    * @param number the value to assign
    * @throw std::out_of_range if either coordinates are out of bounds
    * @throw std::invalid_argument if value is invalid
    */
    void set( Num row, Num col, Num number )
    {
        checkCoords<Dimension>( row, col );
        m_board[row][col].setVal( number );

        updatePossibleValues();
    }
    /**
    * @brief Returns a list of coordinates to possible numbers sorted by
    * possibility list size in ascending order
    * @return a list of coordinates to possible numbers sorted by
    * possibility list size in ascending order
    */
    CoordPossibilitiesList sortedPossibilities()
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
    /**
    * @brief Checks if the board's values are valid, i.e. there are no duplicate
    * values in rows/columns/quadrants or no possible values for some cell.
    * @return true if the board's configuration is valid, false otherwise.
    */
    bool isValid()
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
                    for( Num k = 0; k < Dimension; ++k )
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
        for( Num i = 0; i < Dimension; ++i )
        {
            if( !validateQuadrant( i ) )
            {
                result = false;
                break;
            }
        }

        return result;
    }
    /**
    * @brief Checks if the board is solved, that is, all cells are assigned a value
    * and there are no repeated values in rows/columns/quadrants.
    * @return True if the board is solved, false otherwise.
    */
    bool isSolved() const noexcept
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
    /**
    * @brief Retrieves the offending cells that caused the board to be invalid, in
    * the format (cell1 row, cell1 col, cell2 row, cell2 col, value). If a
    * cell with 0 possible values was the culprit, only one cell is returned.
    *
    * @return the offending cells that caused the board to be invalid,
    */
    std::tuple<Num, Num, Num, Num, Num> offendingVal() const { return m_offendingVal; }
    /**
    * @brief Gets pointers to cells of the specified row
    * @param row the row
    * @return pointers to cells of the specified row
    */
    std::vector<Cell*> getRowCells( Num row )
    {
        std::vector<Cell*> result;

        for( Num i = 0; i < Dimension; ++i )
        {
            Cell* cellPtr = m_board[row].data() + i;
            result.push_back( cellPtr );
        }

        return result;
    }
    /**
    * @brief Gets pointers to cells of the specified column
    * @param col the column
    * @return pointers to cells of the specified column
    */
    std::vector<Cell*> getColCells( Num col )
    {
        std::vector<Cell*> result;

        for( Num i = 0; i < Dimension; ++i )
        {
            Cell* cellPtr = m_board[i].data() + col;
            result.push_back( cellPtr );
        }
        return result;
    }
    /**
    * @brief Gets pointers to cells of the specified quadrant
    * @param quadrant the quadrant
    * @return pointers to cells of the specified quadrant
    */
    std::vector<Cell*> getQuadrantCells( Num quadrant )
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
    /**
    * @brief Equality operator. Two boards are equal if all cell values match.
    * @return True if the boards are equal, false otherwise.
    */
    bool operator==( const Board& rhs ) const
    {
        for( Num i = 0; i < Dimension; ++i )
        {
            for( Num j = 0; j < Dimension; ++j )
            {
                if( at( i, j ) != rhs.at( i, j ) )
                    return false;
            }
        }

        return true;
    }
    /**
    * @brief Inequality operator. Two boards are not equal if some cell value does not match the other's
        @return True if the boards are not equal, false otherwise.
    */
    bool operator!=( const Board& rhs ) const
    {
        return !operator==( rhs );
    }

private:
    std::array<std::array<Cell, Dimension>, Dimension> m_board;
    std::tuple<Num, Num, Num, Num, Num> m_offendingVal;

    /**
    * @brief Performs an actions for each cell of the board.
    * @param func the function to call for each cell. It will be called with the
    * row, column and Cell reference. The function must return true if it should keep
    * being called for the remaining cells.
    */
    void performInCells( std::function<bool( const Num, const Num, Cell& )> func )
    {
        for( Num i = 0; i < Dimension; ++i )
        {
            for( Num j = 0; j < Dimension; ++j )
            {
                if( !func( i, j, m_board[i][j] ) )
                    return;
            }
        }
    }
    /**
    * @brief Performs an actions for each cell of the board.
    * @param func the function to call for each cell. It will be called with the
    * row, column and Cell reference. The function must return true if it should keep
    * being called for the remaining cells.
    */
    void performInCells( std::function<bool( const Num, const Num, const Cell& )> func  ) const
    {
        for( Num i = 0; i < Dimension; ++i )
        {
            for( Num j = 0; j < Dimension; ++j )
            {
                if( !func( i, j, m_board[i][j] ) )
                    return;
            }
        }
    }
    /**
    * @brief Performs an actions for each cell of a quadrant.
    * @param quadrant The quadrant to perform the function in each cell
    * @param func the function to call for each cell. It will be called with the
    * row, column and Cell reference. The function must return true if it should keep
    * being called for the remaining cells.
    */
    void performInQuadrant( Num quadrant, std::function<bool( const Num row, const Num col, Cell& )> func )
    {
        const Num rowMult = quadrant / BlockSide;
        const Num colMult = quadrant % BlockSide;

        const Num startRow = rowMult * BlockSide;
        const Num startCol = colMult * BlockSide;

        for( Num i = startRow; i < startRow + BlockSide; ++i )
        {
            for( Num j = startCol; j < startCol + BlockSide; ++j )
            {
                if( !func( i, j, m_board[i][j] ) )
                    return;
            }
        }
    }
    /**
    * @brief Checks if the specified cell does not have repeated values.
    * @param quadrant the quadrant to check
    * @return True if there are no repeated values in the quadrant.
    */
    bool validateQuadrant( Num quadrant )
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
    /**
    * @brief Causes the board to update possible cell values for
    * the current configuration.
    */
    void updatePossibleValues() noexcept
    {
        bool gotUpdate = false;
        do
        {
            gotUpdate = false;
            for( Num i = 0; i < Dimension; ++i )
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
    /**
    * @brief Causes the board to update possible cell values for
    * the specified row.
    *
    * @param row the row to be updated
    */
    bool updateInRow( Num row ) noexcept
    {
        bool updatedOne = false;

        Nums existingNumbers;
        for( Num i = 0; i < Dimension; ++i )
        {
            if( m_board[row][i].hasVal() )
            {
                existingNumbers.push_back( m_board[row][i].getVal() );
            }
        }

        for( Num i = 0; i < Dimension; ++i )
        {
            auto& cell = m_board[row][i];
            if( !cell.hasVal() )
            {
                updatedOne |= cell.remove( existingNumbers );
            }
        }

        return updatedOne;
    }
    /**
    * @brief Causes the board to update possible cell values for
    * the specified column.
    *
    * @param col the column to be updated
    * @return True if updates occurred, false otherwise
    */
    bool updateInCol( Num col ) noexcept
    {
        Nums existingNumbers;
        bool updatedOne = false;

        for( Num i = 0; i < Dimension; ++i )
        {
            if( m_board[i][col].hasVal() )
            {
                existingNumbers.push_back( m_board[i][col].getVal() );
            }
        }
        for( Num i = 0; i < Dimension; ++i )
        {
            if( !m_board[i][col].hasVal() )
            {
                updatedOne |= m_board[i][col].remove( existingNumbers );
            }
        }

        return updatedOne;
    }
    /**
    * @brief Causes the board to update possible cell values for
    * the specified quadrant.
    *
    * @param quadrant the quadrant to be updated
    * @return True if updates occurred, false otherwise
    */
    bool updateInQuadrant( Num quadrant ) noexcept
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
    /**
    * @brief Causes the board to update possible cell values for
    * the specified group, taking into account cells with
    * 2, 3 and 4 possible values with equal number of cells.
    *
    * @param cells the cells to be updated
    * @return True if updates occurred, false otherwise
    */
    bool updateGroup( const std::vector<Cell*>& group ) noexcept
    {
        bool updatedOne = false;

        // find 2, 3, and 4 subgroups of cells in the provided group
        // with the same possibilities. E.g. two cells with possibilities = { 1, 2},
        // three cells with possibilities = {1, 2, 4}.
        for( Num i = 2; i < Dimension / 2; ++i )
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
};


/**
* @brief Hasher for a board.
*/
template<class BoardType>
class BoardHasher
{
public:
    /**
    * @brief Calculates the hash of the board.
    * @param b the board to calculate the hash for
    * @return the board's hash.
    */
    std::size_t operator()( BoardType const& b ) const noexcept
    {
        size_t seed = 0;

        constexpr auto Dims = b.Dimension;
        constexpr auto leftover = Dims % 8;

        std::vector<Num> leftovers{};

        for( Num i = 0; i < Dims; ++i )
        {
            std::array<Num, 8> rowvals{};

            Num j = 0;
            for( ; j < Dims - leftover; ++j )
            {
                rowvals[j] = b.at( i, j );
            }

            for( ; j < Dims; ++j )
            {
                leftovers.push_back( b.at( i, j ) );
            }

            const auto u64hash = u64Hasher( *( reinterpret_cast< const std::uint64_t* >( rowvals.data() ) ) );
            combineHash( seed, u64hash );
        }

        for( auto& n : leftovers )
        {
            const auto u8hash = u8Hasher( n );
            combineHash( seed, u8hash );
        }

        return seed;
    }

private:
    std::hash<std::uint64_t> u64Hasher;
    std::hash<std::uint8_t> u8Hasher;
    /**
    * @brief boost::hash_combine
    */
    static void combineHash( size_t& seed, const size_t& hash ) noexcept
    {
        seed ^= hash + 0x9e3779b9 + ( seed << 6 ) + ( seed >> 2 );
    }
};

} // namespace

template<std::size_t Dims>
std::ostream& operator<<( std::ostream& stream, const Sudoku::Board<Dims>& board )
{
    for( Sudoku::Num i = 0; i < board.Dimension; ++i )
    {
        for( Sudoku::Num j = 0; j < board.Dimension; ++j )
        {
            stream << std::to_string( board.at( i, j ) ) << " ";
        }
        stream << std::endl;
    }
    return stream;
}
