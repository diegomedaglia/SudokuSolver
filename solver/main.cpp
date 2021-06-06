#include <iostream>
#include <unordered_set>
#include "Board.h"
#include "Utils.h"
#include <iomanip>
#include <chrono>

using std::cout;
using std::endl;

constexpr std::array<std::array<Num, 9>, 9> values{
    {
        // expert
        {0,0,0,0,0,0,0,0,0},
        {5,9,0,0,3,4,6,0,0},
        {0,6,0,0,0,0,0,8,0},
        {4,0,0,0,0,8,0,0,9},
        {0,1,0,0,0,0,0,7,6},
        {0,0,0,0,0,0,5,0,0},
        {0,7,0,9,0,0,0,0,3},
        {3,0,0,8,0,0,2,6,0},
        {0,5,0,0,7,0,0,0,0},
        
        // simple solve
        //{0,0,0,4,5,6,7,8,9},
        //{4,5,6,7,8,9,1,2,3},
        //{7,8,9,1,2,3,4,5,6},
        //{2,3,4,0,6,7,8,9,1},
        //{5,6,7,0,9,1,2,3,4},
        //{8,9,1,0,3,4,5,6,7},
        //{3,4,5,6,7,8,0,0,0},
        //{6,7,8,9,1,2,3,4,5},
        //{9,1,2,3,4,5,6,7,8},
        
        // easy
        //{9,2,6,0,1,5,0,0,0},
        //{1,0,8,7,6,3,9,0,0},
        //{3,0,7,9,0,0,1,6,5},
        //{0,0,9,0,0,0,0,3,8},
        //{7,0,0,0,0,0,6,0,0},
        //{0,0,0,3,9,6,0,0,0},
        //{0,7,0,0,0,0,0,1,6},
        //{6,9,4,0,0,0,7,0,2},
        //{5,0,3,6,0,0,4,0,9},

        // medium
        //{0,1,3,0,2,0,0,6,8},
        //{6,8,0,0,0,0,0,0,0},
        //{2,0,4,0,0,0,0,9,0},
        //{0,0,0,9,4,3,0,5,0},
        //{0,0,0,1,0,0,9,0,0},
        //{0,0,0,6,0,0,4,0,3},
        //{4,0,9,0,3,0,0,0,5},
        //{1,0,2,0,0,5,7,3,0},
        //{5,0,0,0,0,7,0,0,0},

    }
};

bool solve( Board b, std::vector<Board>& visitedStates, int currentLevel )
{
    //cout << std::setw(currentLevel * 2) << "" << "level is " << currentLevel << endl;
    //cout << std::setw( currentLevel * 2 ) << "" << "Board is " << endl << b << endl;

    if( b.isSolved() )
        return true;
    
    //cout << std::setw( currentLevel * 2 ) << "" << "not solved" << endl;

    auto list = b.sortedPossibilities();
        
    for( auto& vals : list )
    {
        for( auto& n : std::get<ccPOSSIBILITIES>( vals ) )
        {
            Board current (b);
            auto row = std::get<ccROW>( vals );
            auto col = std::get<ccCOL>( vals );
            current.set( row, col, n );

            //cout << std::setw( currentLevel * 2 ) << "" << "Trying (" << row << "," << col << ") set to " << static_cast<int>( n ) << endl;
            //cout << std::setw( currentLevel * 2 ) << "" << "board after update: " << endl << current << endl;

            if( current.isValid() )
            {
                if( std::find( visitedStates.begin(), visitedStates.end(), current ) != visitedStates.end() )
                    continue;

                //cout << std::setw( currentLevel * 2 ) << "" << "not yet visited" << endl;

                visitedStates.push_back( current );

                if( solve( current, visitedStates, ++currentLevel ) )
                {
                    return true;
                }
            }
            else
            {
                auto offending = current.offendingVal();
                //cout << std::setw( currentLevel * 2 ) << "" << "NOT valid: " << std::get<0>( offending ) << ", " << std::get<1>( offending ) << ", " << std::get<2>( offending ) << ", " << std::get<3>( offending ) << ", " << std::get<4>( offending ) << endl;
            }
        }
    }
    
    return false;
}

Board solve( Board board )
{
    std::vector<Board> visitedStates;

    if( solve( board, visitedStates, 0 ) )
        return visitedStates.back();

    return {};
}

int main()
{
    Board board( values );

    const auto start = std::chrono::steady_clock::now();

    Board solution = solve( board );

    const auto end = std::chrono::steady_clock::now();

    if( solution.isSolved() )
    {
        std::cout << "solved sudoku!" << std::endl <<
            solution << std::endl;
    }
    else
    {
        std::cout << "Could not solve!" << std::endl;
    }
    
    std::chrono::duration<double> elapsed_seconds = end - start;
    cout << "Took " << elapsed_seconds.count() << "seconds" << endl;

    auto hours = std::chrono::duration_cast< std::chrono::hours >( elapsed_seconds ).count();
    auto minutes = std::chrono::duration_cast< std::chrono::minutes >( elapsed_seconds ).count() % 60;
    auto seconds = std::chrono::duration_cast< std::chrono::seconds >( elapsed_seconds ).count() % 60;

    cout << "( or " << hours << "h " << minutes << "m " << seconds << "s)" << endl;

    return 0;
}
