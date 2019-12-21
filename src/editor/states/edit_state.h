#ifndef EDITOR_STATES_EDIT_STATE_H
#define EDITOR_STATES_EDIT_STATE_H

#include <editor/editor_config.h>
#include <editor/renderer.h>
#include <editor/states/exit_confirmation_state.h>
#include <editor/states/search_state.h>

int edit_state_status_bar_render(editor_config *cfg) {
  status_bar *sb = &(cfg->sb);
  status_bar_wipe(sb);

  int complete = status_bar_render_flag(sb, cfg->dirty) &&
  status_bar_render_c_str(sb, cfg->file_name.data, cfg->file_name.size, 1) &&
  status_bar_render_c_str(sb, " ", 1, 1) &&
  status_bar_render_fraction(sb, cfg->cy + 1, cfg->file_rows.size);
  status_bar_render_final_pad(sb);
  return complete;
}

int edit_state_draw_status_bar(editor_config *cfg) {
  edit_state_status_bar_render(cfg);
  terminal_write("\x1b[7m", 4);
  terminal_write(cfg->sb.rendered.data, cfg->sb.rendered.size);
  terminal_write("\x1b[m", 3);
  return 1;
}

void edit_state_save(editor_config *cfg, const string *file_name) {
  string joined;
  string nl = {"\n", 1, 1};
  string_init(&joined, 1024);
  cfg->file_rows.size--; // We do not want to save the last empty line
  string_vector_join(&(cfg->file_rows), &joined, &nl);
  cfg->file_rows.size++;
  char fname_buf[256];
  string_as_c_str(file_name, fname_buf);
  write_file(fname_buf, &joined);
  cfg->dirty = 0;
}

void edit_state_regular_exit(editor_config *cfg) {
  if (cfg->dirty) {
    exit_confirmation_state_set(cfg);
  } else {
    terminal_clear_screen();
    terminal_flush();
    exit(0);
  }
}

void edit_state_handle_edit_input(editor_config *cfg, int code) {
  if ((size_t) cfg->cy == cfg->file_rows.size - 1) {
    string_vector_append(&(cfg->file_rows), 1);
    string_vector_append(&(cfg->rendered_rows), 1);
  }
  switch (code) {
    case DEL_KEY: {
      if (cfg->file_rows.data[cfg->cy].size != (size_t) cfg->cx) {
        string *cur_row_rendered = &(cfg->rendered_rows.data[cfg->cy]);
        string_erase(&(cfg->file_rows.data[cfg->cy]), cfg->cx, 1);
        render_string(&(cfg->file_rows.data[cfg->cy]), cur_row_rendered, TAB_STOP);
      } else if ((size_t) cfg->cy < (cfg->file_rows.size - 1)) {
        string_append(&(cfg->file_rows.data[cfg->cy]), &(cfg->file_rows.data[cfg->cy + 1]));
        string_vector_pop(&(cfg->file_rows), cfg->cy + 1);
        string_vector_pop(&(cfg->rendered_rows), cfg->cy + 1);
        render_string(&(cfg->file_rows.data[cfg->cy]), &(cfg->rendered_rows.data[cfg->cy]), TAB_STOP);
      } else {
        return;
      }
      break;
    }
    case BACKSPACE: {
      if (0 != (size_t) cfg->cx) {
        cfg->cx -= 1;
        string *cur_row_rendered = &(cfg->rendered_rows.data[cfg->cy]);
        string_erase(&(cfg->file_rows.data[cfg->cy]), cfg->cx, 1);
        render_string(&(cfg->file_rows.data[cfg->cy]), cur_row_rendered, TAB_STOP);
      } else if (cfg->cy != 0) {
        cfg->cy -= 1;
        cfg->cx = cfg->file_rows.data[cfg->cy].size;
        string_append(&(cfg->file_rows.data[cfg->cy]), &(cfg->file_rows.data[cfg->cy]) + 1);
        string_vector_pop(&(cfg->file_rows), cfg->cy + 1);
        string_vector_pop(&(cfg->rendered_rows), cfg->cy + 1);
        render_string(&(cfg->file_rows.data[cfg->cy]), &(cfg->rendered_rows.data[cfg->cy]), TAB_STOP);
      } else {
        return;
      }
      break;
    }
    case '\r': {
      char *new_row_data = cfg->file_rows.data[cfg->cy].data + cfg->cx;
      size_t new_row_size = cfg->file_rows.data[cfg->cy].size - cfg->cx;
      string *new_row = string_vector_insert_c_str(&(cfg->file_rows), cfg->cy + 1, new_row_data, new_row_size);
      string_vector_insert(&(cfg->rendered_rows), cfg->cy + 1, new_row_size);
      cfg->file_rows.data[cfg->cy].size = cfg->cx;
      render_string(&(cfg->file_rows.data[cfg->cy]), &(cfg->rendered_rows.data[cfg->cy]), TAB_STOP);
      render_string(new_row, &(cfg->rendered_rows.data[cfg->cy + 1]), TAB_STOP);
      cfg->cx = 0;
      cfg->cy++;
      break;
    }
    case '\n': {
      break;
    }
    default: {
      string_insert_char(&(cfg->file_rows.data[cfg->cy]), cfg->cx, code);
      render_string(&(cfg->file_rows.data[cfg->cy]), &(cfg->rendered_rows.data[cfg->cy]), TAB_STOP);
      cfg->cx++;
    }
  }
  cfg->dirty = 1;
}

int edit_state_handle_key(editor_config *cfg, int key) {
  switch (key) {
    case CTRL_KEY('q'): {
      edit_state_regular_exit(cfg);
      break;
    }
    case CTRL_KEY('s'): {
      edit_state_save(cfg, &(cfg->file_name));
      break;
    }
    case CTRL_KEY('f'): {
      search_state_set(cfg);
      break;
    }
    case UNKNOWN_KEY:
      return -1;
    case ARROW_UP:
      if (cfg->cy > 0) { cfg->cy--; }
      break;
    case ARROW_DOWN:
      if (cfg->cy < (int) cfg->file_rows.size - 1) { cfg->cy++; }
      break;
    case ARROW_RIGHT:
      if (cfg->cx < (int) cfg->file_rows.data[cfg->cy].size) {
        cfg->cx++;
      } else if (cfg->cy < (int) cfg->file_rows.size - 1) {
        cfg->cy++; cfg->cx = 0;
      }
      break;
    case ARROW_LEFT:
      if (cfg->cx > 0) {
        cfg->cx--;
      } else if (cfg->cy > 0) {
        cfg->cy--;
        cfg->cx = cfg->file_rows.data[cfg->cy].size;
      }
      break;
    case HOME_KEY:
      cfg->cx = 0;
      break;
    case END_KEY:
      cfg->cx = cfg->file_rows.data[cfg->cy].size;
      break;
    case DEL_KEY:
      edit_state_handle_edit_input(cfg, DEL_KEY);
      break;
    case PAGE_UP: { // Pg UP
      int offset = cfg->rows + cfg->cy - cfg->rowoff;
      if (cfg->cy > offset) {
        cfg->cy -= offset;
      } else {
        cfg->cy = 0;
      }
      break;
    }
    case PAGE_DOWN: { // Pg Down
      size_t offset = 2 * cfg->rows - cfg->cy + cfg->rowoff - 1;
      if ((cfg->file_rows.size) > (offset + cfg->cy)) {
        cfg->cy += offset;
      } else {
        cfg->cy = cfg->file_rows.size - 1;
      }
      break;
    }
    default:
      edit_state_handle_edit_input(cfg, key);
  }
  // TODO: this should revert to previous cfg->cx if possible
  if (cfg->cx > (int)cfg->file_rows.data[cfg->cy].size) {
    cfg->cx = cfg->file_rows.data[cfg->cy].size;
  }
  cfg->rx = rendered_pos_from_raw_pos(&(cfg->file_rows.data[cfg->cy]), cfg->cx, TAB_STOP);
  return 0;
}

void edit_state_caret_pos(editor_config *cfg, int *y, int *x) {
  *y = cfg->cy - cfg->rowoff;
  *x = cfg->rx - cfg->coloff;
}

void edit_state_set(editor_config *cfg) {
  cfg->state.handle_key = edit_state_handle_key;
  cfg->state.draw_status_bar = edit_state_draw_status_bar;
  cfg->state.caret_pos = edit_state_caret_pos;
}

#endif /* end of include guard: EDITOR_STATES_EDIT_STATE_H */
