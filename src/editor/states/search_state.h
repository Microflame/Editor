#ifndef EDITOR_STATES_SEARCH_STATE_H
#define EDITOR_STATES_SEARCH_STATE_H

#include <string.h>

#include <editor/editor_config.h>

int search_state_status_bar_render(editor_config *cfg) {
  status_bar *sb = &(cfg->sb);
  status_bar_wipe(sb);

  char message[] = "Search:";

  status_bar_render_c_str(sb, message, sizeof(message) - 1, sizeof(message) - 1);
  status_bar_render_c_str(sb, cfg->state.search_query.data, cfg->state.search_query.size, cfg->state.search_query.size);
  status_bar_render_final_pad(sb);
  return 1;
}

int search_state_draw_status_bar(editor_config *cfg) {
  search_state_status_bar_render(cfg);
  terminal_write(cfg->sb.rendered.data, cfg->sb.rendered.size);
  return 1;
}

int search_state_search(editor_config *cfg) {
  cfg->state.search_query_input_mode = 0;
  char query[128];
  string_as_c_str(&(cfg->state.search_query), query);
  char *start = cfg->file_rows.data[cfg->cy].data + cfg->cx + 1;
  char *pos = strstr(start, query);
  if (pos != NULL) {
    cfg->cx += pos - start;
    return 1;
  }
  size_t row;
  for (row = cfg->cy + 1; row < cfg->file_rows.size; ++row) {
    char *start = cfg->file_rows.data[row].data;
    char *pos = strstr(start, query);
    if (pos != NULL) {
      cfg->cy = row;
      cfg->cx = pos - start;
      return 1;
    }
  }
  cfg->state.search_query_input_mode = 1;
  return 0;
}

void edit_state_set(editor_config *);
void search_state_handle_edit_input(editor_config *cfg, int code) {
  if (code == ESC_KEY) {
    edit_state_set(cfg);
    return;
  }
  if (code == '\r') {
    search_state_search(cfg);
    cfg->rx = rendered_pos_from_raw_pos(&(cfg->file_rows.data[cfg->cy]), cfg->cx, TAB_STOP);
    return;
  }
  if (!cfg->state.search_query_input_mode) {
    return;
  }
  switch (code) {
    case '\n':
    case '\t':
    case DEL_KEY:
    return;
    case BACKSPACE: {
      if (0 != cfg->state.search_query.size) {
        string_resize(&(cfg->state.search_query), cfg->state.search_query.size - 1);
      }
      break;
    }
    default: {
      if (cfg->state.search_query.size < (size_t) cfg->cols - 10) {
        string_append_char(&(cfg->state.search_query), code, 1);
      }
    }
  }
}

int search_state_handle_key(editor_config *cfg, int key) {
  switch (key) {
    case UNKNOWN_KEY:
    case ARROW_UP:
    case ARROW_DOWN:
    case ARROW_RIGHT:
    case ARROW_LEFT:
    case HOME_KEY:
    case END_KEY:
    case DEL_KEY:
    case PAGE_UP:
    case PAGE_DOWN:
      return -1;
    default:
      search_state_handle_edit_input(cfg, key);
  }
  return 0;
}

void search_state_caret_pos(editor_config *cfg, int *y, int *x) {
  if (cfg->state.search_query_input_mode) {
    *y = cfg->rows;
    *x = 7 + cfg->state.search_query.size;
  } else {
    *y = cfg->cy - cfg->rowoff;
    *x = cfg->rx - cfg->coloff;
  }
}

void search_state_set(editor_config *cfg) {
  cfg->state.search_query_input_mode = 1;
  string_resize(&(cfg->state.search_query), 0);
  cfg->state.handle_key = search_state_handle_key;
  cfg->state.draw_status_bar = search_state_draw_status_bar;
  cfg->state.caret_pos = search_state_caret_pos;
}

#endif /* end of include guard: EDITOR_STATES_SEARCH_STATE_H */
