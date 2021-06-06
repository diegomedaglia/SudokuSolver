#pragma once
#include "Common.h"

class Cell
{
public:
    Cell();
    bool hasVal() const noexcept;
    Num getVal() const noexcept;
    void setVal( Num val ) noexcept;
    bool remove( const Nums& possibilities ) noexcept;
    bool remove( Num n ) noexcept;
    Nums possibilities() const noexcept;
    void possibilities( const Nums& possibilities );
    Num row() const noexcept { return m_row; }
    void row( Num row ) noexcept { m_row = row; }
    Num col() const noexcept { return m_col; }
    void col( Num col ) noexcept { m_col = col; }

    bool operator==( const Cell& rhs ) const;
    bool operator!=( const Cell& rhs ) const;
private:
    Nums m_possibilities;
    Num m_row;
    Num m_col;
};

using Cells = std::vector<Cell>;
