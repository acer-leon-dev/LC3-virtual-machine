#include "lc3-vm.h"
#include <cstdio>

void setup_vm()
{
    // Since exactly one condition flag should be set at any given
    // time, set the Z flag
    register_storage[R_COND] = FL_ZRO;
    // Move PC to default start position
    register_storage[R_PC] = PC_START;
}

bool read_image(char * path)
{
    return true;
}

uint16_t mem_read(uint16_t address)
{
    // switch (address) {
    //     case MR_KBSR: {
    //         if (check_key()) {
    //             memory_storage.at(MR_KBSR) = (1 << 15);
    //             memory_storage.at(MR_KBDR) = std::getchar();
    //         }
    //         else {
    //             memory_storage.at(MR_KBSR) = 0;
    //         }    
    //     }
    // }

    return memory_storage.at(address);

}

void mem_write(uint16_t address, uint16_t value)
{
    memory_storage[address] = value;
}

uint16_t sign_extend(uint16_t x, int bit_count)
{
    if ((x >> (bit_count - 1)) & 0b1) {
        x |= (0xFFFF << bit_count);
    }
    return x;
}

void update_flags(Register reg  )
{
    if (register_storage[reg] == 0) {
        register_storage[R_COND] = FL_ZRO;
    }
    // a 1 in the left-most bit indicates negative
    else if (register_storage[reg] >> 15)
    {
        register_storage[R_COND] = FL_NEG;
    }
    else
    {
        register_storage[R_COND] = FL_POS;
    }
}