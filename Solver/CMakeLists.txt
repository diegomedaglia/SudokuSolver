cmake_minimum_required (VERSION 3.11 )

set( SOURCES 
    main.cpp
)

add_executable( Solver ${SOURCES} )
target_link_libraries(Solver Sudoku)
