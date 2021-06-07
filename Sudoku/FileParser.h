#pragma once

#include <iostream>
#include <string>
#include "Board.h"

namespace Sudoku
{

/**
* Parses a file containing a representation of a board. Cells are separated
* by whitespace and have values in the range [0, 9]. 0 denotes empty cells.
* @param filename the path to the file
* @return A board with the values specified in the file
* @throw std::invalid_argument The filename can't be opened for reading
* @throw std::runtime_error An error occurred during parsing of the file.
*/
Board parseFile( const std::string& filename );

}