#include <iostream>
#include <chrono>
#include <string>
#include <sstream>
#include "FileParser.h"
#include "Solver.h"
#include "Utils.h"

int main(int argc, char* argv[] )
{
    if( argc < 3 )
    {
        std::cerr << "Usage: " << argv[0] << " <region side length> <filename>" << std::endl << std::endl;
        return 1;
    }

    Sudoku::Num blockSize = 0;

    try
    {
        auto size = std::stoul( argv[1], nullptr, 0 );
        if( size > std::numeric_limits<Sudoku::Num>::max() )
        {
            throw std::out_of_range( []() { 
                std::stringstream ss( "Max value supported is " ); 
                ss << std::numeric_limits<Sudoku::Num>::max(); 
                return ss.str(); 
                }() );
        }

        blockSize = static_cast<Sudoku::Num>( size );
    }
    catch( const std::exception& ex )
    {
        std::cerr << ex.what() << std::endl;
        return 1;
    }

    Sudoku::Board board( blockSize );
    try
    {
        board = Sudoku::parseFile( blockSize, argv[2] );
    }
    catch( std::exception& ex )
    {
        std::cerr << "Failed to read file: " << ex.what() << std::endl << std::endl;
        return 2;
    }

    const auto start = std::chrono::steady_clock::now();

    auto s = Sudoku::solve( board );

    const auto end = std::chrono::steady_clock::now();

    if( s.isSolved() )
    {
        std::cout << "solved sudoku!" << std::endl <<
            s << std::endl;
    }
    else
    {
        std::cout << "Could not solve!" << std::endl;
    }
    
    std::chrono::duration<double> elapsed_seconds = end - start;
    
    auto hours = std::chrono::duration_cast< std::chrono::hours >( elapsed_seconds ).count();
    auto minutes = std::chrono::duration_cast< std::chrono::minutes >( elapsed_seconds ).count() % 60;
    auto seconds = std::chrono::duration_cast< std::chrono::seconds >( elapsed_seconds ).count() % 60;

    std::cout << "Took " << hours << "h " << minutes << "m " << seconds << "s" << std::endl;

    return 0;
}

