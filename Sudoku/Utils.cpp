#include "Utils.h"
#include <string>

std::ostream& operator<<( std::ostream& stream, const Board& board )
{
    for( int i = 0; i < DIMS; ++i )
    {
        for( int j = 0; j < DIMS; ++j )
        {
            stream << std::to_string( board.at( i, j ) ) << " ";
        }
        stream << std::endl;
    }
    return stream;
}
