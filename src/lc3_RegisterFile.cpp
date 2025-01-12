#include "lc3_RegisterFile.h"
#include "lc3_LC3.h"


namespace lc3
{

lc3_size_t RegisterFile::get(register_t reg)
{
    return internal.at(reg);
}

void RegisterFile::set(register_t reg, lc3_size_t val)
{
    internal.at(reg) = val;
}

void RegisterFile::inc(register_t reg, lc3_size_t val)
{
    internal.at(reg) += val;
}

void RegisterFile::dec(register_t reg, lc3_size_t val)
{
    internal.at(reg) -= val;
}

}