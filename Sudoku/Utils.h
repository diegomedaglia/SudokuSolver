#pragma once
#include "Board.h"

std::ostream& operator<<( std::ostream& stream, const Board& board );

void checkCoords( Num line, Num col );
void checkCoord( Num coord );
int getQuadrant( Num row, Num col );