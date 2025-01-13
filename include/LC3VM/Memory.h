#ifndef DC137FD0_EFCC_45E9_B07B_755FB8666238
#define DC137FD0_EFCC_45E9_B07B_755FB8666238

#include "LC3VM/Precompiled.h"
#include "LC3VM/Types.h"

namespace lc3
{

class Memory
{
public:
    lc3_size_t read(lc3_size_t address);
    void write(lc3_size_t address, lc3_size_t value);
private:
    std::array<lc3_size_t, MEMORY_MAX> m_internal;
private:
    friend class LC3;
};

}

#endif /* DC137FD0_EFCC_45E9_B07B_755FB8666238 */
