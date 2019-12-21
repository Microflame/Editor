#ifndef KEYCODE_H
#define KEYCODE_H

enum key_codes {
  BACKSPACE = 127,
  UNKNOWN_KEY = 1024,
  ARROW_LEFT,
  ARROW_RIGHT,
  ARROW_UP,
  ARROW_DOWN,
  DEL_KEY,
  HOME_KEY,
  END_KEY,
  PAGE_UP,
  PAGE_DOWN,
  ESC_KEY
};

int handle_ecaped_key(const unsigned char *bytes, size_t len, int *num_parsed) {
  if (len == 0) {
    return ESC_KEY;
  }
  if (bytes[0] != '[') {
    return UNKNOWN_KEY;
  }
  *num_parsed += 1;
  if (len == 1) {
    return UNKNOWN_KEY;
  }
  *num_parsed += 1;
  switch (bytes[1]) {
    case 'A': return ARROW_UP;
    case 'B': return ARROW_DOWN;
    case 'C': return ARROW_RIGHT;
    case 'D': return ARROW_LEFT;
    case 'H': return HOME_KEY;
    case 'F': return END_KEY;
  }
  if (len == 2) {
    return UNKNOWN_KEY;
  }
  *num_parsed += 1;
  if (bytes[2] == '~') {
    switch (bytes[1]) {
      case '3': return DEL_KEY;
      case '5': return PAGE_UP;
      case '6': return PAGE_DOWN;
      case '1':
      case '7': return HOME_KEY;
      case '8': return END_KEY;
    }
  }
  return UNKNOWN_KEY;
}

#endif /* end of include guard: KEYCODE_H */
