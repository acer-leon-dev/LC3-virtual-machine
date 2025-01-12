#ifndef C7476CD9_F8B1_4EBF_8E67_12D85842D54A
#define C7476CD9_F8B1_4EBF_8E67_12D85842D54A

#include "lc3_precompiled.h"
#include "lc3_types.h"

namespace lc3
{

class RegisterFile
{
private:
    lc3_size_t get(register_t reg);
    void set(register_t reg, lc3_size_t val);
    void inc(register_t reg, lc3_size_t val);
    void dec(register_t reg, lc3_size_t val);
private:
    std::array<lc3_size_t, NUM_REGISTERS> internal;
private:
    friend class LC3;
};

}

#endif /* C7476CD9_F8B1_4EBF_8E67_12D85842D54A */
