#include "LC3VM/Bit.h"


namespace lc3::bit {

lc3_size_t sext(lc3_size_t x, uint32_t bit_count)
{
    if ((x >> (bit_count - 1)) & 0b1)
    {
        x |= 0b1111111111111111 << bit_count;
    }
    return x;
}

// lc3_size_t zext(lc3_size_t x, uint32_t bit_count) 
// {
//     return x;
// }

lc3_size_t cut(lc3_size_t instruction, uint32_t left, uint32_t right)
{
    if (left == label::msb)
    {
        left = sizeof(lc3_size_t) * 8 - 1;
    }

    if (left < right)
    {
        throw std::domain_error("Error with 'lc3::bits::grab_argument(lc3_size_t, int, int)': left index cannot be less than right index");
    }

    lc3_size_t arg = instruction >> right;
    arg &= (1 << (left - right + 1)) - 1;
    return arg;
}

lc3_size_t get_arg(lc3_size_t instruction, uint32_t bit)
{
    return cut(instruction, bit, bit);
}

}