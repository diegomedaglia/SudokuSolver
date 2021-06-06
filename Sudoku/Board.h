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
    Board();
    Board( const std::array<std::array<Num, 9>, 9>& values );
    Board( const Board& other );
    Board& operator=( const Board& other );

    void updatePossibleValues() noexcept;
    bool updateInRow( Num row ) noexcept;
    bool updateInCol( Num col ) noexcept;
    bool updateInQuadrant( Num quadrant ) noexcept;
    bool updateGroup( const std::vector<Cell*>& cells ) noexcept;
    std::vector<Cell*> getRowCells( int row );
    std::vector<Cell*> getColCells( int col );
    std::vector<Cell*> getQuadrantCells( int quadrant );

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

    void performInCells( std::function<bool( const int, const int, Cell&)>);
    void performInCells( std::function<bool( const int, const int, const Cell& )> ) const;
    void performInQuadrant( Num quadrant, std::function<bool( const int row, const int col, Cell& )> func );
    void performInRow( int row, std::function<bool( const int, const int, Cell& )> func );
    void performInCol( int col, std::function<bool( const int, const int, Cell& )> func );





    //friend struct std::hash<Board>;
};
