#ifndef C7476CD9_F8B1_4EBF_8E67_12D85842D54A
#define C7476CD9_F8B1_4EBF_8E67_12D85842D54A

#include "LC3VM/Precompiled.h"
#include "LC3VM/Types.h"

namespace lc3
{

class RegisterFile
{
public:
    lc3_size_t get(register_t reg);
    void set(register_t reg, lc3_size_t val);
    void inc(register_t reg, lc3_size_t val);
    void dec(register_t reg, lc3_size_t val);
    void update_flags(register_t reg);
    void update_flags();
private:
    friend class LC3;
private:
    std::array<lc3_size_t, NUm_reg> m_internal;
    register_t m_last_set;
    void m_set_last_set(register_t reg);
};

}

#endif /* C7476CD9_F8B1_4EBF_8E67_12D85842D54A */
