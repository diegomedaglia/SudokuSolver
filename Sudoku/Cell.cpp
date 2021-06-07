#include <algorithm>
#include <array>

#include "Cell.h"
#include "Utils.h"

static constexpr std::array<Num, DIMS> startingPossibilities{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };

Cell::Cell() :
    m_possibilities( startingPossibilities.begin(), startingPossibilities.end() )
{
}

Nums Cell::possibilities() const noexcept
{
    return m_possibilities;
}

void Cell::possibilities( const Nums& possibilities )
{
    m_possibilities = possibilities;
}

bool Cell::operator==( const Cell& rhs ) const
{
    return m_possibilities == rhs.possibilities();
}

bool Cell::operator!=( const Cell& rhs ) const
{
    return !( operator==( rhs ) );
}

bool Cell::remove( const Nums& possibilities ) noexcept
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

bool Cell::remove( Num n ) noexcept
{
    const auto size = m_possibilities.size();
    m_possibilities.erase(
        std::remove( m_possibilities.begin(), m_possibilities.end(), n ), 
        m_possibilities.end() 
    );

    return size != m_possibilities.size();
}

bool Cell::hasVal() const noexcept
{
    return m_possibilities.size() == 1;
}

Num Cell::getVal() const noexcept
{
    if( hasVal() )
        return m_possibilities.front();

    return 0;
}

void Cell::setVal( Num val ) noexcept
{
    checkValue( val );
    m_possibilities = { val };
}
