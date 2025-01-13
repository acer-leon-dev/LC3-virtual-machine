#include "LC3VM/LC3.h"

#if defined(_WIN32)
 #include <windows.h>
 #include <conio.h>
#elif defined(__unix__)
 #include <cstdlib>
 #include <unistd.h>
 #include <fcntl.h>
 #include <sys/time.h>
 #include <sys/types.h>
 #include <sys/termios.h>
 #include <sys/mman.h>
#endif

namespace
{

uint32_t getchar_no_echo()
{ 
   #if defined(_WIN32)
    return getch(); 
   #elif defined(__unix__)
    // POSIX - termios.h
    // Get the current terminal settings
    termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    // Disable echoing
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    // Read a single character
    char ch = getchar();

    // Restore the original terminal settings
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    return ch;
   #endif
}

}


namespace lc3
{

void LC3::m_trap_GETC()
{
    m_reg.set(R_R0, getchar_no_echo());
    m_reg.update_flags();
}

void LC3::m_trap_OUT()
{
    std::print("{:c}", m_reg.get(R_R0));
    std::cout.flush();
}

void LC3::m_trap_PUTS()
{
    lc3_size_t address = m_reg.get(R_R0);
    lc3_size_t ch = m_mem.read(address);
    for (; ch != 0; address++)
    {
        ch = m_mem.read(address);
        std::print("{:c}", ch);
    }
    std::cout.flush();
}

void LC3::m_trap_IN()
{
    std::print("Enter a character: ");
    lc3_size_t ch = std::getchar();
    std::cout.flush();
    m_reg.set(R_R0, ch);
    m_reg.update_flags();
}

void LC3::m_trap_PUTSP()
{
    lc3_size_t address = m_reg.get(R_R0);
    lc3_size_t word = m_mem.read(address);
    for (; word != 0; address++)
    {
        word = m_mem.read(address);

        char ch1 = word & 0b11111111;
        std::print("{}", ch1);

        if (char ch2 = (word >> 8))
        {
            std::print("{}", ch2);
        }
    }
}

void LC3::m_trap_HALT()
{
    std::println("\nHALT");
    std::cout.flush();
    m_running = false;
}

}