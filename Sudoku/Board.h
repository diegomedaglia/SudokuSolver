#pragma once
#include <ostream>
#include <array>
#include <utility>
#include <functional>
#include "Common.h"
#include "Cell.h"

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
*/
class Board
{
public:
    /**
    * @brief Contructs an empty board (i.e. all cells unassigned).
    */
    Board() = default;
    /**
    * @brief Contructs a board with the values provided.
    */
    Board( const std::array<std::array<Num, DIMS>, DIMS>& values );
    /**
    * @brief Copy constructor
    */
    Board( const Board& other );
    /**
    * @brief Copy assignment
    */
    Board& operator=( const Board& other );
    /**
    * @brief Returns the assigned value for the specified cell. 0 is returned
    * for cells with no assigned value.
    *
    * @param row the cell row
    * @param col the cell column
    * @return The cell's value
    * @throw std::out_of_range if either coordinates are out of bounds
    */
    Num at( Num row, Num col ) const;
    /**
    * @brief Returns a copy of the cell at the specified location.
    *
    * @param row the cell row
    * @param col the cell column
    * @return A copy of the cell instance
    * @throw std::out_of_range if either coordinates are out of bounds
    */
    Cell cell( Num row, Num col ) const;
    /**
    * @brief Assigns a value to the cell at the specified location.
    *
    * @param row the cell row
    * @param col the cell column
    * @param number the value to assign
    * @throw std::out_of_range if either coordinates are out of bounds
    * @throw std::invalid_argument if value is invalid
    */
    void set( Num row, Num col, Num number );
    /**
    * @brief Returns a list of coordinates to possible numbers sorted by
    * possibility list size in ascending order
    * @return a list of coordinates to possible numbers sorted by
    * possibility list size in ascending order
    */
    CoordPossibilitiesList sortedPossibilities();
    /**
    * @brief Checks if the board's values are valid, i.e. there are no duplicate
    * values in rows/columns/quadrants or no possible values for some cell.
    * @return true if the board's configuration is valid, false otherwise.
    */
    bool isValid();
    /**
    * @brief Checks if the board is solved, that is, all cells are assigned a value
    * and there are no repeated values in rows/columns/quadrants.
    * @return True if the board is solved, false otherwise.
    */
    bool isSolved() const noexcept;
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
    std::vector<Cell*> getRowCells( Num row );
    /**
    * @brief Gets pointers to cells of the specified column
    * @param col the column
    * @return pointers to cells of the specified column
    */
    std::vector<Cell*> getColCells( Num col );
    /**
    * @brief Gets pointers to cells of the specified quadrant
    * @param quadrant the quadrant
    * @return pointers to cells of the specified quadrant
    */
    std::vector<Cell*> getQuadrantCells( Num quadrant );
    /**
    * @brief Equality operator. Two boards are equal if all cell values match.
    * @return True if the boards are equal, false otherwise.
    */
    bool operator==( const Board& rhs ) const;
    /**
    * @brief Inequality operator. Two boards are not equal if some cell value does not match the other's
        @return True if the boards are not equal, false otherwise.
    */
    bool operator!=( const Board& rhs ) const;

private:
    std::array<std::array<Cell, DIMS>, DIMS> m_board;
    std::tuple<Num, Num, Num, Num, Num> m_offendingVal;

    /**
    * @brief Performs an actions for each cell of the board.
    * @param func the function to call for each cell. It will be called with the
    * row, column and Cell reference. The function must return true if it should keep
    * being called for the remaining cells.
    */
    void performInCells( std::function<bool( const Num, const Num, Cell& )> func );
    /**
    * @brief Performs an actions for each cell of the board.
    * @param func the function to call for each cell. It will be called with the
    * row, column and Cell reference. The function must return true if it should keep
    * being called for the remaining cells.
    */
    void performInCells( std::function<bool( const Num, const Num, const Cell& func )> ) const;
    /**
    * @brief Performs an actions for each cell of a quadrant.
    * @param quadrant The quadrant to perform the function in each cell
    * @param func the function to call for each cell. It will be called with the
    * row, column and Cell reference. The function must return true if it should keep
    * being called for the remaining cells.
    */
    void performInQuadrant( Num quadrant, std::function<bool( const Num row, const Num col, Cell& )> func );
    /**
    * @brief Checks if the specified cell does not have repeated values.
    * @param quadrant the quadrant to check
    * @return True if there are no repeated values in the quadrant.
    */
    bool validateQuadrant( Num quadrant );
    /**
    * @brief Causes the board to update possible cell values for
    * the current configuration.
    */
    void updatePossibleValues() noexcept;
    /**
    * @brief Causes the board to update possible cell values for
    * the specified row.
    *
    * @param row the row to be updated
    */
    bool updateInRow( Num row ) noexcept;
    /**
    * @brief Causes the board to update possible cell values for
    * the specified column.
    *
    * @param col the column to be updated
    * @return True if updates occurred, false otherwise
    */
    bool updateInCol( Num col ) noexcept;
    /**
    * @brief Causes the board to update possible cell values for
    * the specified quadrant.
    *
    * @param quadrant the quadrant to be updated
    * @return True if updates occurred, false otherwise
    */
    bool updateInQuadrant( Num quadrant ) noexcept;
    /**
    * @brief Causes the board to update possible cell values for
    * the specified group, taking into account cells with
    * 2, 3 and 4 possible values with equal number of cells.
    *
    * @param cells the cells to be updated
    * @return True if updates occurred, false otherwise
    */
    bool updateGroup( const std::vector<Cell*>& cells ) noexcept;
};


/**
* @brief Hasher for a board.
*/
class BoardHasher
{
public:
    /**
    * @brief Calculates the hash of the board.
    * @param b the board to calculate the hash for
    * @return the board's hash.
    */
    std::size_t operator()( Board const& b ) const noexcept;

private:
    std::hash<std::uint64_t> u64Hasher;
    std::hash<std::uint8_t> u8Hasher;
    /**
    * @brief boost::hash_combine
    */
    static void combineHash( size_t& seed, const size_t& hash ) noexcept;
};

} // namespace

namespace std
{
    /**
    * @copydoc Sudoku::BoardHasher
    */
    template<>
    struct hash<Sudoku::Board>
    {
        std::size_t operator()( Sudoku::Board const& b ) const noexcept
        {
            return hasher( b );
        }
        Sudoku::BoardHasher hasher;
    };
}

std::ostream& operator<<( std::ostream& stream, const Sudoku::Board& board );
