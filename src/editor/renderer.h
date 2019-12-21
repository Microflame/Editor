#ifndef RENDERER_H
#define RENDERER_H

#include <data_struct/string.h>
#include <editor/tokenizer.h>
#include <editor/colorizer.h>

int tab_size_for_position(int pos, int tab_stop) {
  return (pos + 1) % tab_stop + 1;
}

int render_string(const string *src, string *dst, int tab_stop) {
  string_resize(dst, 0);
  size_t num_rendered = 0;
  size_t i = 0;
  // char str[src->size + 1];
  // string_as_c_str(src, str);
  // while (str[it] != '\0') {
  //   int size;
  //   int type;
  //   get_token_type(str + it, &size, &type, &tok);
  //   if (str[it] == '\t') {
  //     int tab_size = tab_size_for_position(num_rendered, tab_stop);
  //     string_append_char(dst, ' ', tab_size);
  //     num_rendered += tab_size;
  //   } else {
  //     num_rendered += put_colorized(dst, str + it, size, type);
  //   }
  //   it += size;
  // }
  for (i = 0; i < src->size; ++i) {
    if (src->data[i] == '\t') {
      int tab_size = tab_size_for_position(num_rendered, tab_stop);
      string_append_char(dst, ' ', tab_size);
      num_rendered += tab_size;
    } else {
      string_append_char(dst, src->data[i], 1);
      num_rendered++;
    }
  }
  return num_rendered;
}

int rendered_pos_from_raw_pos(const string *src, int pos, int tab_stop) {
  int rendered_pos = 0;
  int i;
  for (i = 0; i < pos; ++i) {
    if (src->data[i] == '\t') {
      int tab_size = tab_size_for_position(rendered_pos, tab_stop);
      rendered_pos += tab_size;
    } else {
      rendered_pos++;
    }
  }
  return rendered_pos;
}

#endif /* end of include guard: RENDERER_H */
