#pragma once
#include <ostream>
#include <array>
#include <map>
#include <utility>
#include <functional>
#include "Common.h"
#include "Cell.h"

namespace Sudoku
{

/**
* Structure used to store a board coordinate and a list
* of possible values for that Cell.
*/
struct CoordPossibilities
{
    int row;
    int col;
    Nums possibilities;
};

using CoordPossibilitiesList = std::vector<CoordPossibilities>;

/**
* Represents the Sudoku board and provides operations to manipulate its values.
*/
class Board
{
public:
    /**
    * Contructs an empty board (i.e. all cells unassigned).
    */
    Board();
    /**
    * Contructs a board with the values provided.
    */
    Board( const std::array<std::array<Num, DIMS>, DIMS>& values );
    /**
    * Copy constructor
    */
    Board( const Board& other );
    /**
    * Copy assignment
    */
    Board& operator=( const Board& other );
    /**
    * Returns the assigned value for the specified cell. 0 is returned
    * for cells with no assigned value.
    *
    * @param row the cell row
    * @param col the cell column
    * @return The cell's value
    * @throw std::out_of_range if either coordinates are out of bounds
    */
    Num at( int row, int col ) const;
    /**
    * Returns a copy of the cell at the specified location.
    *
    * @param row the cell row
    * @param col the cell column
    * @return A copy of the cell instance
    * @throw std::out_of_range if either coordinates are out of bounds
    */
    Cell cell( int row, int col ) const;
    /**
    * Assigns a value to the cell at the specified location.
    *
    * @param row the cell row
    * @param col the cell column
    * @param number the value to assign
    * @throw std::out_of_range if either coordinates are out of bounds
    * @throw std::invalid_argument if value is invalid
    */
    void set( int row, int col, Num number );
    /**
    * Returns a list of coordinates to possible numbers sorted by
    * possibility list size in ascending order
    * @return a list of coordinates to possible numbers sorted by
    * possibility list size in ascending order
    */
    CoordPossibilitiesList sortedPossibilities();
    /**
    * Checks if the board's values are valid, i.e. there are no duplicate
    * values in rows/columns/quadrants or no possible values for some cell.
    * @return true if the board's configuration is valid, false otherwise.
    */
    bool isValid();
    /**
    * Checks if the board is solved, that is, all cells are assigned a value
    * and there are no repeated values in rows/columns/quadrants.
    * @return True if the board is solved, false otherwise.
    */
    bool isSolved() const noexcept;
    /**
    * Retrieves the offending cells that caused the board to be invalid, in
    * the format (cell1 row, cell1 col, cell2 row, cell2 col, value). If a
    * cell with 0 possible values was the culprit, only one cell is returned.
    *
    * @return the offending cells that caused the board to be invalid,
    */
    std::tuple<int, int, int, int, Num> offendingVal() const { return m_offendingVal; }
    /**
    * Gets pointers to cells of the specified row
    * @param row the row
    * @return pointers to cells of the specified row
    */
    std::vector<Cell*> getRowCells( int row );
    /**
    * Gets pointers to cells of the specified column
    * @param col the column
    * @return pointers to cells of the specified column
    */
    std::vector<Cell*> getColCells( int col );
    /**
    * Gets pointers to cells of the specified quadrant
    * @param quadrant the quadrant
    * @return pointers to cells of the specified quadrant
    */
    std::vector<Cell*> getQuadrantCells( int quadrant );
    /**
    * Equality operator. Two boards are equal if all cell values match.
    * @return True if the boards are equal, false otherwise.
    */
    bool operator==( const Board& rhs ) const;
    /**
    * Inequality operator. Two boards are not equal if some cell value does not match the other's
        @return True if the boards are not equal, false otherwise.
    */
    bool operator!=( const Board& rhs ) const;

private:
    std::array<std::array<Cell, DIMS>, DIMS> m_board;
    std::tuple<int, int, int, int, Num> m_offendingVal;

    /**
    * Performs an actions for each cell of the board.
    * @param func the function to call for each cell. It will be called with the
    * row, column and Cell reference. The function must return true if it should keep
    * being called for the remaining cells.
    */
    void performInCells( std::function<bool( const int, const int, Cell& )> func );
    /**
    * Performs an actions for each cell of the board.
    * @param func the function to call for each cell. It will be called with the
    * row, column and Cell reference. The function must return true if it should keep
    * being called for the remaining cells.
    */
    void performInCells( std::function<bool( const int, const int, const Cell& func )> ) const;
    /**
    * Performs an actions for each cell of a quadrant.
    * @param quadrant The quadrant to perform the function in each cell
    * @param func the function to call for each cell. It will be called with the
    * row, column and Cell reference. The function must return true if it should keep
    * being called for the remaining cells.
    */
    void performInQuadrant( Num quadrant, std::function<bool( const int row, const int col, Cell& )> func );
    /**
    * Checks if the specified cell does not have repeated values.
    * @param quadrant the quadrant to check
    * @return True if there are no repeated values in the quadrant.
    */
    bool validateQuadrant( int quadrant );
    /**
    * Causes the board to update possible cell values for
    * the current configuration.
    */
    void updatePossibleValues() noexcept;
    /**
    * Causes the board to update possible cell values for
    * the specified row.
    *
    * @param row the row to be updated
    */
    bool updateInRow( Num row ) noexcept;
    /**
    * Causes the board to update possible cell values for
    * the specified column.
    *
    * @param col the column to be updated
    * @return True if updates occurred, false otherwise
    */
    bool updateInCol( Num col ) noexcept;
    /**
    * Causes the board to update possible cell values for
    * the specified quadrant.
    *
    * @param quadrant the quadrant to be updated
    * @return True if updates occurred, false otherwise
    */
    bool updateInQuadrant( Num quadrant ) noexcept;
    /**
    * Causes the board to update possible cell values for
    * the specified group, taking into account cells with
    * 2, 3 and 4 possible values with equal number of cells.
    *
    * @param cells the cells to be updated
    * @return True if updates occurred, false otherwise
    */
    bool updateGroup( const std::vector<Cell*>& cells ) noexcept;
};


/**
* Hasher for a board.
*/
class BoardHasher
{
public:
    /**
    * Calculates the hash of the board.
    * @param b the board to calculate the hash for
    * @return the board's hash.
    */
    std::size_t operator()( Board const& b ) const noexcept;

    /**
    * boost::hash_combine
    */
    static void combineHash( size_t& seed, const size_t& hash ) noexcept;;
private:
    std::hash<std::uint64_t> u64Hasher;
    std::hash<std::uint8_t> u8Hasher;
};

} // namespace

namespace std
{

    /**
    * Inject our hasher in namespace std by specializing std::hash.
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
