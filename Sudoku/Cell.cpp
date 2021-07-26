#include "Cell.h"
#include "Utils.h"

using namespace Sudoku;

Cell::Cell( Num dims ) :
    m_dims( dims ),
    m_possibilities( m_dims )
{
    std::iota<Nums::iterator, Num>( m_possibilities.begin(), m_possibilities.end(), 1 );
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
    checkValue( m_dims, val );
    m_possibilities = { val };
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

Nums Cell::possibilities() const noexcept
{
    return m_possibilities;
}

void Cell::possibilities( const Nums& possibilities )
{
    m_possibilities = possibilities;
}

bool Cell::operator==( const Cell& rhs ) const noexcept
{
    return m_possibilities == rhs.possibilities();
}

bool Cell::operator!=( const Cell& rhs ) const noexcept
{
    return !( operator==( rhs ) );
}
