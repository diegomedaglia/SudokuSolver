#pragma once
#include "Common.h"

class Cell
{
public:
    Cell();

    bool hasVal() const noexcept;
    Num getVal() const noexcept;
    void setVal( Num val ) noexcept;
    void remove( const Nums& possibilities ) noexcept;
    void remove( Num n ) noexcept;
    Nums possibilities() const noexcept;
    void possibilities( const Nums& possibilities );
private:
    Nums m_possibilities;
};

using Cells = std::vector<Cell>;
