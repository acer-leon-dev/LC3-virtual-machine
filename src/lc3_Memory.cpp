#include "lc3_Memory.h"
#include "lc3_LC3.h"


namespace lc3 {

lc3_size_t Memory::read(lc3_size_t address) {
    switch (address) {
        case MR_KBSR: {
            if (LC3::m_check_key()) {
                internal.at(MR_KBSR) = (1 << 15);
                internal.at(MR_KBDR) = std::getchar();
            }
            else {
                internal.at(MR_KBSR) = 0;
            }    
        }
    }

    return internal.at(address);
}

void Memory::write(lc3_size_t address, lc3_size_t value) {
        internal.at(address) = value;
}

}