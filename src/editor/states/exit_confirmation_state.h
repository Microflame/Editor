#ifndef EDITOR_STATE_EXIT_CONFIRMATION_STATE_H
#define EDITOR_STATE_EXIT_CONFIRMATION_STATE_H

#include <editor/editor_config.h>

void edit_state_set(editor_config *);
int exit_confirmation_state_handle_key(editor_config *cfg, int key) {
  switch (key) {
    case 'y':
    case 'Y':
    terminal_clear_screen();
    terminal_flush();
    exit(0);
    break;
    default:
    edit_state_set(cfg);
  }
  return 0;
}

int exit_confirmation_state_draw_status_bar(editor_config *cfg) {
    char message[] = "Do you want to discard unsaved changes? (y/n)";
  status_bar_wipe(&(cfg->sb));
  status_bar_render_c_str(&(cfg->sb), message, sizeof(message) - 1, sizeof(message) - 1);
  status_bar_render_final_pad(&(cfg->sb));
  terminal_write(cfg->sb.rendered.data, cfg->sb.rendered.size);
  return 0;
}

void exit_confirmation_state_caret_pos(editor_config *cfg, int *y, int *x) {
  *y = cfg->rows;
  *x = 45;
}

void exit_confirmation_state_set(editor_config *cfg) {
  cfg->state.handle_key = exit_confirmation_state_handle_key;
  cfg->state.draw_status_bar = exit_confirmation_state_draw_status_bar;
  cfg->state.caret_pos = exit_confirmation_state_caret_pos;
}

#endif /* end of include guard: EDITOR_STATE_EXIT_CONFIRMATION_STATE_H */
