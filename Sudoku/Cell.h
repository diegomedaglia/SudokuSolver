#pragma once
#include <numeric>

#include "Common.h"
#include "Utils.h"

namespace Sudoku
{

/**
* @brief Represents a cell of a sudoku board.
* @tparam Dims the number of possibilities the cell has ([1..Dims])
*/
template<std::size_t Dims>
class Cell
{
public:
    /**
    * @brief Empty constructor.
    */
    Cell() :
        m_possibilities( Dims )
    {
        std::iota<Nums::iterator, Num>( m_possibilities.begin(), m_possibilities.end(), 1 );
    }
    /**
    * @brief Checks if the cell has an assigned value, i.e. it only has
    * one possibility.
    * @return True if the cell has a value.
    */
    bool hasVal() const noexcept
    {
        return m_possibilities.size() == 1;
    }
    /**
    * @brief Gets the assigned value of the cell. If there are multiple
    * or no possibilities, returns 0;
    * @return 0 if there are multiple or no possibilities, 1..9 otherwise
    */
    Num getVal() const noexcept
    {
        if( hasVal() )
            return m_possibilities.front();

        return 0;
    }
    /**
    * @brief Sets the value of the cell.
    * @param val the value
    */
    void setVal( Num val ) noexcept
    {
        checkValue<Dims>( val );
        m_possibilities = { val };
    }
    /**
    * @brief Removes a list of possible values from this cell's possible values.
    * @param possiblilities the possibilities to remove
    * @return True if there were possibilities removed, false otherwise.
    */
    bool remove( const Nums& possibilities ) noexcept
    {
        const auto size = m_possibilities.size();
        m_possibilities.erase(
            std::remove_if( m_possibilities.begin(), m_possibilities.end(),
                [&possibilities]( const Num& number )
                {
                    return contains( possibilities, number );
                }
        ), m_possibilities.end() );

        return size != m_possibilities.size();
    }
    /**
    * @brief Removes a possible values from this cell's possible values.
    * @param n the possibility to remove
    * @return True if there were possibilities removed, false otherwise.
    */
    bool remove( Num n ) noexcept
    {
        const auto size = m_possibilities.size();
        m_possibilities.erase(
            std::remove( m_possibilities.begin(), m_possibilities.end(), n ),
            m_possibilities.end()
        );

        return size != m_possibilities.size();
    }
    /**
    * @brief Retrieves the possible values for this cell.
    * @return the possible values for this cell.
    */
    Nums possibilities() const noexcept
    {
        return m_possibilities;
    }
    /**
    * @brief Sets the possible values for this cell.
    * @param possibilities the possible values for this cell.
    */
    void possibilities( const Nums& possibilities )
    {
        m_possibilities = possibilities;
    }
    /**
    * @brief Compares two cells for equality
    * @param rhs the cell to compare to this cell
    * @return true if both cells have the same possible values, false otherwise
    */
    bool operator==( const Cell& rhs ) const noexcept
    {
        return m_possibilities == rhs.possibilities();
    }
    /**
    * @brief Compares two cells for inequality
    * @param rhs the cell to compare to this cell
    * @return true if the cells do not have the same possible values, false otherwise
    */
    bool operator!=( const Cell& rhs ) const noexcept
    {
        return !( operator==( rhs ) );
    }
private:
    Nums m_possibilities;
};

template<std::size_t Dims>
using Cells = std::vector<Cell<Dims>>;

}
