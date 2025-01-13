#include "LC3VM/Memory.h"
#include "LC3VM/VM.h"


namespace lc3 {

lc3_size_t Memory::read(lc3_size_t address) {
    switch (address) {
        case MR_KBSR: {
            if (LC3::s_check_key()) {
                m_internal.at(MR_KBSR) = (1 << 15);
                m_internal.at(MR_KBDR) = std::getchar();
            }
            else {
                m_internal.at(MR_KBSR) = 0;
            }    
        }
    }

    return m_internal.at(address);
}

void Memory::write(lc3_size_t address, lc3_size_t value) {
        m_internal.at(address) = value;
}

}