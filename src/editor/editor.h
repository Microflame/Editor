#ifndef EDITOR_H
#define EDITOR_H

#include <unistd.h>

#define TAB_STOP 4

#include <utils/termutils.h>
#include <utils/keycode.h>
#include <utils/file_io.h>
#include <data_struct/string_vector.h>
#include <editor/editor_config.h>
#include <editor/renderer.h>
#include <editor/states/edit_state.h>

editor_config econf;

void editor_scroll() {
  if (econf.cy < econf.rowoff) {
    econf.rowoff = econf.cy;
  }
  if (econf.cy >= (econf.rowoff + econf.rows)) {
    econf.rowoff = econf.cy - econf.rows + 1;
  }
  if (econf.rx < econf.coloff) {
    econf.coloff = econf.rx;
  }
  if (econf.rx >= (econf.coloff + econf.cols)) {
    econf.coloff = econf.rx - econf.cols + 1;
  }
}

int draw_rows() {
  //TODO: init once
  tokenizer tok;
  tokenizer_init(&tok);

  int i = 0;
  for (i = 0; i < econf.rows; ++i) {
    int file_row = i + econf.rowoff;
    if (file_row < (int) econf.file_rows.size) {
      int line_len = econf.rendered_rows.data[file_row].size - econf.coloff;
      line_len = line_len < 0 ? 0 : line_len;
      int line_visible_len = line_len > econf.cols ? econf.cols : line_len;
      const char *visible_line = econf.rendered_rows.data[file_row].data + econf.coloff;

      int types[econf.rendered_rows.data[file_row].size];
      tokenize_str(&econf.rendered_rows.data[file_row], types, &tok);

      string colorized = colorize(visible_line, types + econf.coloff, line_visible_len);
      terminal_write(colorized.data, colorized.size);
    } else {
      terminal_write("~", 1);
    }
    terminal_write("\x1b[K", 3);
    terminal_write("\r\n", 2);
  }
  return 1;
}

int refresh_screen() {
  editor_scroll();

  terminal_write("\x1b[?25l", 6);
  terminal_write("\x1b[H", 3);

  draw_rows();
  econf.state.draw_status_bar(&econf);

  int y, x;
  econf.state.caret_pos(&econf, &y, &x);
  terminal_place_caret(y, x);

  terminal_write("\x1b[?25h", 6);
  terminal_flush();
  return 1;
}

int handle_event(unsigned char *bytes, size_t size) {
  int num_parsed = 1;
  if (size == 0) {
    return 0;
  }
  int key = bytes[0] == '\x1b' ? handle_ecaped_key(bytes + 1, size - 1, &num_parsed) : bytes[0];

  if (econf.state.handle_key(&econf, key) == -1) {
    return -1;
  }

  return num_parsed;
}

int handle_bytes(unsigned char *bytes, size_t size) {
  int num_handle;
  int num_handled = 0;
  while ((num_handle = handle_event(bytes + num_handled, size - num_handled)) > 0) {
    num_handled += num_handle;
  }
  if (num_handle == -1) {
    num_handled = size;
  }
  refresh_screen();
  return num_handled;
}

int editor_open(const char *fname) {
  read_file(fname, &(econf.file_rows));
  size_t i;
  for (i = 0; i < econf.file_rows.size; ++i) {
    const string *cur_str = &(econf.file_rows.data[i]);
    string *rendered = string_vector_append(&(econf.rendered_rows), cur_str->size);
    render_string(cur_str, rendered, TAB_STOP);
  }
  return 1;
}

int editor_init(const char *fname) {
  econf.cx = 0;
  econf.cy = 0;
  econf.rowoff = 0;
  econf.dirty = 0;
  edit_state_set(&econf);
  string_init_with_c_str(&(econf.file_name), fname, strlen(fname));
  string_vector_init(&(econf.file_rows), 1);
  string_vector_init(&(econf.rendered_rows), 1);

  editor_open(fname);

  if (terminal_get_window_size(&econf.rows, &econf.cols) == -1) {
    die("get_window_size");
  }
  econf.rows--;
  status_bar_init(&(econf.sb), econf.cols);
  terminal_clear_screen();
  refresh_screen();
  return 1;
}

#endif /* end of include guard: EDITOR_H */
