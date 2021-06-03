#include "Board.h"

constexpr std::array<std::array<Num, 9>, 9> values{
    {
        {0,0,0,0,0,0,0,0,0},
        {5,9,0,0,3,4,6,0,0},
        {0,6,0,0,0,0,0,8,0},
        {4,0,0,0,0,8,0,0,9},
        {0,1,0,0,0,0,0,7,6},
        {0,0,0,0,0,0,5,0,0},
        {0,7,0,9,0,0,0,0,3},
        {3,0,0,8,0,0,2,6,0},
        {0,5,0,0,7,0,0,0,0},
    }
};

int main()
{
    Board b( values );

    std::vector<Board> visitedStates;

    CoordCellList list = b.sortedPossibilities();

    while( !list.empty() )
    {
            
    }
}