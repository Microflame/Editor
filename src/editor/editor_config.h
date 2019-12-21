#ifndef EDITOR_CONFIG_H
#define EDITOR_CONFIG_H

#include <data_struct/string.h>
#include <data_struct/string_vector.h>
#include <editor/states/editor_state.h>
#include <editor/status_bar.h>

struct editor_config {
  int rows;
  int cols;
  int cx, cy, rx;
  int rowoff;
  int coloff;
  int dirty;
  string_vector file_rows;
  string_vector rendered_rows;
  string file_name;
  status_bar sb;
  editor_state state;
};
typedef struct editor_config editor_config;

#endif /* end of include guard: EDITOR_CONFIG_H */
