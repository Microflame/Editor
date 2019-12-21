#ifndef STATUS_BAR_H
#define STATUS_BAR_H

// #include <string.h>
#include <stdio.h>

#include <data_struct/string.h>

struct status_bar {
  size_t size;
  string rendered;
  size_t remainig;
};
typedef struct status_bar status_bar;

void status_bar_init(status_bar *sb, size_t size);

int status_bar_render_flag(status_bar *sb, int flag);

int status_bar_render_c_str(status_bar *sb, const char *str, size_t len, size_t min_len);

int status_bar_render_fraction(status_bar *sb, int first, int second);

void status_bar_render_final_pad(status_bar *sb);

void status_bar_wipe(status_bar *sb);

#endif /* end of include guard: STATUS_BAR_H */
