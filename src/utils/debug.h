#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>
#include <ctype.h>

void log_input_byte(char c) {
  if (iscntrl(c)) {
    printf("%d\r\n", c);
  } else {
    printf("%d ('%c')\r\n", c, c);
  }
}

void die(const char *str) {
  write(STDOUT_FILENO, "\x1b[2J", 4);
  write(STDOUT_FILENO, "\x1b[H", 3);
  
  perror(str);
  exit(1);
}

#endif /* end of include guard: DEBUG_H */
