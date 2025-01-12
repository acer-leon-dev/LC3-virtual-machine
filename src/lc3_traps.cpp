#include "LC3VM/lc3.h"

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
    m_registers.set(R_R0, getchar_no_echo());
    m_registers.update_flags();
}

void LC3::m_trap_OUT()
{
    std::print("{}", static_cast<char>(m_registers.get(R_R0)));
    std::cout.flush();
}

void LC3::m_trap_PUTS()
{
    lc3_size_t address = m_registers.get(R_R0);
    char ch = m_memory.read(address);
    for (; ch != '\0'; address++)
    {
        ch = m_memory.read(address);
        std::print("{}", ch);
    }
    std::cout.flush();
}

void LC3::m_trap_IN()
{
    std::print("Enter a character: ");
    char ch = std::getchar();
    std::cout.flush();
    m_registers.set(R_R0, static_cast<lc3_size_t>(ch));
    m_registers.update_flags();
}

void LC3::m_trap_PUTSP()
{
    lc3_size_t address = m_registers.get(R_R0);
    lc3_size_t word = m_memory.read(address);
    for (; word != 0x0000; address++)
    {
        word = m_memory.read(address);

        char ch1 = word & 0xFF;
        std::print("{}", ch1);

        char ch2 = (word >> 8);
        if (ch2)
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