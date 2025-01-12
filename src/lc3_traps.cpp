#include "lc3.h"

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

namespace {

uint32_t getchar_no_echo() { 
   #if defined(_WIN32)
    char ch = getch(); 
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
   #endif
    return ch;
}

}

namespace lc3 {

void LC3::m_trap_GETC() {
    m_registers.set(R_R0, static_cast<uint16_t>(getchar_no_echo()));
    m_update_flags(m_registers.get(R_R0));
}


void LC3::m_trap_OUT() {
    std::print("{}", static_cast<char>(m_registers.get(R_R0)));
    std::cout.flush();
}


void LC3::m_trap_PUTS() {
    lc3_size_t address = m_registers.get(R_R0);
    for (lc3_size_t word = m_memory.read(address); word != 0x00; word = m_memory.read(++address)) {
        std::print("{}", static_cast<char>(word));
    }
    std::cout.flush();
}


void LC3::m_trap_IN() {
    std::print("Enter a character: ");
    char ch = std::getchar();
    std::cout << std::flush;
    m_registers.set(R_R0, static_cast<lc3_size_t>(ch));
    m_update_flags(m_registers.get(R_R0));
}


void LC3::m_trap_PUTSP() {
    lc3_size_t address = m_registers.get(R_R0);
    for (lc3_size_t word = m_memory.read(address); word != 0x0000; word = m_memory.read(++address)) {
        std::print("{}", static_cast<char>(word & 0xFF));
        if ((word >> 8) & 0xFF) {
            std::print("{}", static_cast<char>((word >> 8)));
        }
    }
    std::cout.flush();
}


void LC3::m_trap_HALT() {
    std::println("\nHALT");
    std::cout.flush();
    running = false;
}

}