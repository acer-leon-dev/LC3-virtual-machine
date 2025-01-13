#include "LC3VM/RegisterFile.h"
#include "LC3VM/VM.h"


namespace lc3
{

lc3_size_t RegisterFile::get(register_t reg)
{
    return m_internal.at(reg);
}

void RegisterFile::set(register_t reg, lc3_size_t val)
{
    m_internal.at(reg) = val;
    m_set_last_set(reg);
}

void RegisterFile::inc(register_t reg, lc3_size_t val)
{
    m_internal.at(reg) += val;
    m_set_last_set(reg);
}

void RegisterFile::dec(register_t reg, lc3_size_t val)
{
    m_internal.at(reg) -= val;
    m_set_last_set(reg);
}

void RegisterFile::update_flags(register_t reg) 
{
    lc3_size_t val = m_internal.at(reg);
    if (val == 0) // zero
    {
        m_internal.at(R_COND) = FL_ZRO;
    }
    else if (val >> 15) // negative
    {
        m_internal.at(R_COND) = FL_NEG;
    }
    else // positive
    {
        m_internal.at(R_COND) = FL_POS;
    }
}

void RegisterFile::update_flags()
{
    update_flags(m_last_set);
}

void RegisterFile::m_set_last_set(register_t reg)
{
    if (R_R0 <= reg || reg <= R_R7)
    {
        m_last_set = reg;
    }
}
}