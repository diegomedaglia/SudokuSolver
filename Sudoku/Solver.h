#pragma once
#include "Board.h"

namespace Sudoku
{
    /**
    * @brief Solves the given board using backtracking.
    * @param board The board to solve.
    * @return The solved board, or 'board' if no solution was found.
    */
    Board solve( Board board );
}
