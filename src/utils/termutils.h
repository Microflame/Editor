#ifndef TERMUTILS_H
#define TERMUTILS_H

#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/select.h>
#include <sys/ioctl.h>

#include <data_struct/string.h>
#include <utils/debug.h>

#define CTRL_KEY(x) ((x) & 0x1f)

struct terminal_control{
  struct termios original_termios;
  string write_buffer;
} TC;

void terminal_disable_raw_mode() {
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &TC.original_termios) == -1) {
    die("tcsetattr");
  }
}

void terminal_enable_raw_mode() {
  atexit(terminal_disable_raw_mode);

  struct termios current_termios = TC.original_termios;
  current_termios.c_lflag &= ~ECHO; // Stop terminal from printing keys
  current_termios.c_lflag &= ~ICANON; // Disable canonical mode so every keypress is consumed instantly
  current_termios.c_lflag &= ~ISIG; // Disable conversion of Ctrl-[C,Z] to signals
  current_termios.c_lflag &= ~IEXTEN; // Disable flow control

  current_termios.c_iflag &= ~IXON; // Disable Ctrl-V
  current_termios.c_iflag &= ~ICRNL; // Disable converiosn of carriage return to new line

  current_termios.c_oflag &= ~OPOST; // Disable \n -> \n\r output conversion

  current_termios.c_cflag |= (CS8); // Set char size to 8 bytes

  current_termios.c_iflag &= ~BRKINT; // Disable SIGINT on break condition
  current_termios.c_iflag &= ~INPCK; // Disable parity check (whatever this is)
  current_termios.c_iflag &= ~ISTRIP; // Set every 8th bit to 0

  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &current_termios) == -1) {
    die("tcsetattr");
  }
}

void terminal_init() {
  if (tcgetattr(STDIN_FILENO, &TC.original_termios) == -1) {
    die("tcsetattr");
  }
  string_init(&TC.write_buffer, 128);
}

int terminal_read(int fd, void *buf, size_t count, float timeout) {
  const int MICROSECONDS_IN_SECOND = 1000000;

  struct timeval tv;
  int time_sec = timeout;
  int time_usec = (timeout - time_sec) * MICROSECONDS_IN_SECOND;
  tv.tv_sec = time_sec;
  tv.tv_usec = time_usec;
  fd_set fds;
  FD_ZERO(&fds);
  FD_SET(fd, &fds);
  select(fd+1, &fds, NULL, NULL, &tv);
  if (FD_ISSET(0, &fds)) {
    errno = 0;
    return read(fd, buf, count);
  } else {
    errno = EAGAIN;
    return -1;
  }
}

int terminal_write(const char *s, size_t size) {
  string_append_c_str(&TC.write_buffer, s, size);
  return size;
}

int terminal_flush() {
  size_t num_write_total = 0;
  int num_write = 0;
  while (num_write_total < TC.write_buffer.size) {
    if ((num_write = write(STDOUT_FILENO,
      TC.write_buffer.data + num_write_total,
      TC.write_buffer.size - num_write_total)) <= 0) {
      die("terminal_flush");
    }
    num_write_total += num_write;
  }
  TC.write_buffer.size = 0;
  return 0;
}

void terminal_clear_screen() {
  terminal_write("\x1b[2J", 4);
  terminal_write("\x1b[H", 3);
}

int terminal_get_window_size(int *rows, int *cols) {
  struct winsize ws;

  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1
      || ws.ws_col == 0
      || ws.ws_row == 0) {
    return -1;
  } else {
    *cols = ws.ws_col;
    *rows = ws.ws_row;
    return 0;
  }
}

void terminal_place_caret(int y, int x) {
  char buf[32];
  size_t size = snprintf(buf, sizeof(buf), "\x1b[%d;%dH", y + 1, x + 1);
  terminal_write(buf, size);
}

#endif /* end of include guard: TERMUTILS_H */
