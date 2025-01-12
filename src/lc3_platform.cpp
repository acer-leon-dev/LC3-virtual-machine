#include "LC3VM/lc3.h"

#if defined(_WIN32)
 #include <windows.h>
 #include <conio.h>
#elif defined(__unix__)
 #include <unistd.h>
 #include <fcntl.h>
 #include <sys/time.h>
 #include <sys/types.h>
 #include <sys/termios.h>
 #include <sys/mman.h>
#endif


namespace lc3 {

#if defined(_WIN32)
 HANDLE hStdin = INVALID_HANDLE_VALUE;
 DWORD fdwMode, fdwOldMode;
#elif defined(__unix__)
 termios original_tio;
#endif

void LC3::s_disable_input_buffering()
{
  #if defined(_WIN32)
   hStdin = GetStdHandle(STD_INPUT_HANDLE);
   GetConsoleMode(hStdin, &fdwOldMode); /* save old mode */
   fdwMode = fdwOldMode
         ^ ENABLE_ECHO_INPUT  /* no input echo */
         ^ ENABLE_LINE_INPUT; /* return when one or
                                 more characters are available */
   SetConsoleMode(hStdin, fdwMode); /* set new mode */
   FlushConsoleInputBuffer(hStdin); /* clear buffer */
  #elif defined(__unix__)
   tcgetattr(STDIN_FILENO, &original_tio);
   termios new_tio = original_tio;
   new_tio.c_lflag &= ~ICANON & ~ECHO;
   tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);
  #endif

}

void LC3::s_restore_input_buffering()
{
  #if defined(_WIN32)
   SetConsoleMode(hStdin, fdwOldMode);
  #elif defined(__unix__)
   tcsetattr(STDIN_FILENO, TCSANOW, &original_tio);
  #endif
}

bool LC3::s_check_key() {
  #if defined(_WIN32)
   return WaitForSingleObject(hStdin, 1000) == WAIT_OBJECT_0 && _kbhit();
  #elif defined(__unix__)
   fd_set readfds;
   FD_ZERO(&readfds);
   FD_SET(STDIN_FILENO, &readfds);

   timeval timeout;
   timeout.tv_sec = 0;
   timeout.tv_usec = 0;
   return select(1, &readfds, NULL, NULL, &timeout) != 0;
  #endif
}

void LC3::s_handle_interrupt(int signal)
{
   s_restore_input_buffering();
   std::println();
   std::exit(-2);
}

}