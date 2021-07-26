#include "BoardHasher.h"

using Sudoku::BoardHasher;
using Sudoku::Board;

std::size_t BoardHasher::operator()( Board const& b ) const noexcept
{
    size_t seed = 0;

    for( Num i = 0; i < b.dimension(); ++i )
    {
        for( Num j = 0; j < b.dimension(); ++j )
        {
            const auto u64hash = u64Hasher( b.at( i, j ) );
            combineHash( seed, u64hash );
        }
    }

    return seed;
}

void BoardHasher::combineHash( size_t& seed, const size_t& hash ) noexcept
{
    seed ^= hash + 0x9e3779b9 + ( seed << 6 ) + ( seed >> 2 );
}
