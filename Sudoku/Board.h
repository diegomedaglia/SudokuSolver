#pragma once
#include <ostream>
#include <array>
#include <map>
#include <utility>
#include "Common.h"
#include "Cell.h"

using CoordCell = std::tuple<int, int, Cell>;
using CoordCellList = std::vector<CoordCell>;

enum CoordCellMembers 
{
    ccROW,
    ccCOL,
    ccCELL
};

class Board
{
public:
    Board() = default;
    Board( const std::array<std::array<Num, 9>, 9>& values );

    void updatePossibleValues() noexcept;
    bool updateInRow( Num row ) noexcept;
    bool updateInCol( Num col ) noexcept;
    bool updateInQuadrant( Num quadrant ) noexcept;
    Num at( int row, int col ) const;
    Cell cell( int row, int col ) const;
    void set( int row, int col, Num number );
    CoordCellList sortedPossibilities();

private:
    std::array<std::array<Cell, DIMS>, DIMS> m_board;
};
