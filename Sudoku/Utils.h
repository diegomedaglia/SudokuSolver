#pragma once
#include "Common.h"

namespace Sudoku
{

void checkCoords( Num line, Num col );
void checkCoord( Num coord );
void checkValue( Num value );
int getQuadrant( Num row, Num col );

inline bool contains( const Nums& nums, Num val )
{
    return std::find( nums.begin(), nums.end(), val ) != nums.end();
}

}
