cmake_minimum_required (VERSION 3.11)

set( SOURCES 
    "Cell.h" 
    "Board.h" 
    "Common.h"
    "Utils.h"  
    "FileParser.h" 
    "Solver.h"
    )
    
add_library ( Sudoku INTERFACE )

target_include_directories( Sudoku 
                INTERFACE
                "${CMAKE_CURRENT_SOURCE_DIR}"
)
