#pragma once
#include "Common.h"

namespace Sudoku
{

/**
* @brief Represents a cell of a sudoku board.
*/
class Cell
{
public:
    /**
    * @brief Empty constructor.
    */
    Cell();
    /**
    * @brief Checks if the cell has an assigned value, i.e. it only has
    * one possibility.
    * @return True if the cell has a value.
    */
    bool hasVal() const noexcept;
    /**
    * @brief Gets the assigned value of the cell. If there are multiple
    * or no possibilities, returns 0;
    * @return 0 if there are multiple or no possibilities, 1..9 otherwise
    */
    Num getVal() const noexcept;
    /**
    * @brief Sets the value of the cell.
    * @param val the value
    */
    void setVal( Num val ) noexcept;
    /**
    * @brief Removes a list of possible values from this cell's possible values.
    * @param possiblilities the possibilities to remove
    * @return True if there were possibilities removed, false otherwise.
    */
    bool remove( const Nums& possibilities ) noexcept;
    /**
    * @brief Removes a possible values from this cell's possible values.
    * @param n the possibility to remove
    * @return True if there were possibilities removed, false otherwise.
    */
    bool remove( Num n ) noexcept;
    /**
    * @brief Retrieves the possible values for this cell.
    * @return the possible values for this cell.
    */
    Nums possibilities() const noexcept;
    /**
    * @brief Sets the possible values for this cell.
    * @param possibilities the possible values for this cell.
    */
    void possibilities( const Nums& possibilities );
    /**
    * @brief Compares two cells for equality
    * @param rhs the cell to compare to this cell
    * @return true if both cells have the same possible values, false otherwise
    */
    bool operator==( const Cell& rhs ) const noexcept;
    /**
    * @brief Compares two cells for inequality
    * @param rhs the cell to compare to this cell
    * @return true if the cells do not have the same possible values, false otherwise
    */
    bool operator!=( const Cell& rhs ) const noexcept;
private:
    Nums m_possibilities;
};

using Cells = std::vector<Cell>;

}
