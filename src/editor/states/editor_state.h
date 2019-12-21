#ifndef EDITOR_STATES_EDITOR_STATE_H
#define EDITOR_STATES_EDITOR_STATE_H

#include <data_struct/string.h>

typedef struct editor_config editor_config;

struct editor_state {
  int (*handle_key)(editor_config *, int);
  int (*draw_status_bar)(editor_config *);
  void (*caret_pos)(editor_config *, int *, int *);

  // Search
  string search_query;
  int search_query_input_mode;
};
typedef struct editor_state editor_state;

void editor_state_init(editor_state *es) {
  string_init(&(es->search_query), 1);
}

#endif /* end of include guard: EDITOR_STATES_EDITOR_STATE_H */
