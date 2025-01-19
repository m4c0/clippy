module;
#if LECO_TARGET_WINDOWS
// If we include <conio.h>, we can't define a symbol named getch
extern "C" int _getch();
#else
#include <stdio.h>
#include <termios.h>
#endif

export module getch;

export int getch() {
#if LECO_TARGET_WINDOWS
  return _getch();
#else
  termios o {};
  termios n {};
  tcgetattr(0, &o);
  
  n = o;
  n.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(0, TCSANOW, &n);

  int res = getchar();
  tcsetattr(0, TCSANOW, &o);
  return res;
#endif
}
