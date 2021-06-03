#pragma once
#include <ostream>
#include <array>
#include <map>
#include <utility>
#include <functional>
#include "Common.h"
#include "Cell.h"

using CoordPossibilities = std::tuple<int, int, Nums>;
using CoordPossibilitiesList = std::vector<CoordPossibilities>;

enum CoordCellMembers 
{
    ccROW = 0,
    ccCOL,
    ccPOSSIBILITIES
};

class Board
{
public:
    Board() = default;
    Board( const std::array<std::array<Num, 9>, 9>& values );
    Board( const Board& other );
    Board& operator=( const Board& other );

    void updatePossibleValues() noexcept;
    bool updateInRow( Num row ) noexcept;
    bool updateInCol( Num col ) noexcept;
    bool updateInQuadrant( Num quadrant ) noexcept;
    Num at( int row, int col ) const;
    Cell cell( int row, int col ) const;
    void set( int row, int col, Num number );
    CoordPossibilitiesList sortedPossibilities();
    bool isValid();
    bool isSolved();
    std::tuple<int, int, int, int, Num> offendingVal() const { return m_offendingVal; }
    bool operator==( const Board& rhs ) const;
    bool operator!=( const Board& rhs ) const;

private:
    std::array<std::array<Cell, DIMS>, DIMS> m_board;
    std::tuple<int, int, int, int, Num> m_offendingVal;

    bool validateInQuadrant( Num row, Num col );

    void performInCells( std::function<bool(int, int, Cell&)>);
    void performInCells( std::function<bool( int, int, const Cell& )> ) const;
    void performInQuadrant( Num quadrant, std::function<bool( int row, int col )> func );
};
