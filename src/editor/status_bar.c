#include <editor/status_bar.h>

#include <data_struct/string.h>

int num_digits_in_int(int num) {
  int num_digits = num < 0;
  while (num) {
    num /= 10;
    num_digits++;
  }
  return num_digits;
}

void status_bar_init(status_bar *sb, size_t size) {
  sb->size = size;
  string_init(&(sb->rendered), size);
}

int status_bar_render_flag(status_bar *sb, int flag) {
  if (sb->remainig < 3) {
    return 0;
  }
  string_append_char(&(sb->rendered), '[', 1);
  if (flag) {
    string_append_char(&(sb->rendered), '*', 1);
  } else {
    string_append_char(&(sb->rendered), ' ', 1);
  }
  string_append_char(&(sb->rendered), ']', 1);
  sb->remainig -= 3;
  return 1;
}

int status_bar_render_c_str(status_bar *sb, const char *str, size_t len, size_t min_len) {
  if (sb->remainig < len) {
    if (sb->remainig < min_len + 3) {
      return 0;
    }
    string_append_c_str(&(sb->rendered), str, sb->remainig - 3);
    string_append_char(&(sb->rendered), '.', 3);
    sb->remainig = 0;
    return 1;
  }
  string_append_c_str(&(sb->rendered), str, len);
  sb->remainig -= len;
  return 1;
}

int status_bar_render_fraction(status_bar *sb, int first, int second) {
  char buf[128];
  int module_size = snprintf(buf, sizeof(buf), "%d/%d", first, second);
  if (module_size < 0) {
    return 0;
  }
  if (sb->remainig < (size_t) module_size) {
    return 0;
  }
  string_append_c_str(&(sb->rendered), buf, module_size);
  sb->remainig -= module_size;
  return 1;
}

void status_bar_render_final_pad(status_bar *sb) {
  string_append_char(&(sb->rendered), ' ', sb->remainig);
  sb->remainig = 0;
}

void status_bar_wipe(status_bar *sb) {
  sb->remainig = sb->size;
  string_resize(&(sb->rendered), 0);
}
