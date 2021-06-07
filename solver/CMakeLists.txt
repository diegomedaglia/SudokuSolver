
set( SOURCES 
    main.cpp
    Solver.cpp
    Solver.h
)

add_executable( Solver ${SOURCES} )
target_link_libraries(Solver Sudoku)
