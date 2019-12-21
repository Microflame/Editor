#ifndef EDITOR_COLORIZER_H
#define EDITOR_COLORIZER_H

#include <data_struct/string.h>

#include <editor/tokenizer.h>

int put_colorized(string *dst, const char *str, int len, int type) {
  switch (type) {
    case OPERATOR_TOKEN:
    case KEYWORD_TOKEN: {
      const char color[] = "\x1b[38;5;198m";
      string_append_c_str(dst, color, strlen(color));
      string_append_c_str(dst, str, len);
      string_append_c_str(dst, "\x1b[39m", 5);
      break;
    }
    case DEFAULT_TYPE_TOKEN: {
      const char color[] = "\x1b[38;5;117m";
      string_append_c_str(dst, color, strlen(color));
      string_append_c_str(dst, str, len);
      string_append_c_str(dst, "\x1b[39m", 5);
      break;
    }
    // case IDENTIFIER_TOKEN: {
    //   const char color[] = "\x1b";
    //   string_append_c_str(dst, color, strlen(color));
    //   string_append_c_str(dst, str, len);
    //   string_append_c_str(dst, "\x1b[39m", 5);
    //   break;
    // }
    case CONSTANT_TOKEN: {
      const char color[] = "\x1b[38;5;220m";
      string_append_c_str(dst, color, strlen(color));
      string_append_c_str(dst, str, len);
      string_append_c_str(dst, "\x1b[39m", 5);
      break;
    }
    case STRING_TOKEN: {
      const char color[] = "\x1b[38;5;78m";
      string_append_c_str(dst, color, strlen(color));
      string_append_c_str(dst, str, len);
      string_append_c_str(dst, "\x1b[39m", 5);
      break;
    }
    case COMMENT_TOKEN: {
      const char color[] = "\x1b[38;5;39m";
      string_append_c_str(dst, color, strlen(color));
      string_append_c_str(dst, str, len);
      string_append_c_str(dst, "\x1b[39m", 5);
      break;
    }
    default:
    string_append_c_str(dst, str, len);
  }
  return len;
}

string colorize(const char *str, const int *types, size_t len) {
  string colorized;
  string_init(&colorized, 1);
  size_t begin_it = 0;
  while (begin_it < len) {
    size_t fragment_len = 0;
    int cur = types[begin_it + fragment_len];
    while (((begin_it + fragment_len) < len) && (types[begin_it + fragment_len] == cur)) ++fragment_len;
    put_colorized(&colorized, str + begin_it, fragment_len, cur);
    begin_it += fragment_len;
  }
  return colorized;
}

#endif /* end of include guard: EDITOR_COLORIZER_H */
