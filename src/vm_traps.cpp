#include "lc3-vm.h"
#include <cstdio>
#include <print>
#include <memory>

void trap_puts()
{
    /* one char per word */
    uint16_t* c = memory_storage.data() + register_storage[R_R0];
    while (*c)
    {
        putc((char)*c, stdout);
        ++c;
    }
    std::fflush(stdout);
}

void trap_getc()
{
    /* read a single ASCII char */
    register_storage[R_R0] = (uint16_t)getchar();
    update_flags(R_R0);
}

void trap_in()
{
    std::print("Enter a character: ");
    char c = std::getchar();
    std::putc(c, stdout);
    std::fflush(stdout);
    register_storage[R_R0] = (uint16_t)c;
    update_flags(R_R0);
}

void trap_putsp()
{
    /* one char per byte (two bytes per word)
       here we need to swap back to
       big endian format */
    uint16_t* c = memory_storage.data() + register_storage[R_R0];
    while (*c)
    {
        char char1 = (*c) & 0xFF;
        putc(char1, stdout);
        char char2 = (*c) >> 8;
        if (char2) putc(char2, stdout);
        ++c;
    }
    fflush(stdout);
}

void trap_halt()
{
    puts("HALT");
    fflush(stdout);
    // running = 0;
}