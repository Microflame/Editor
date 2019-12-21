#include <data_struct/string.h>

#include <string.h>

void string_free(string *s) {
  free(s->data);
}

void string_init(string *s, size_t capacity) {
  s->data = malloc(capacity);
  s->size = 0;
  s->capacity = capacity;
}

void string_init_with_c_str(string *s, const char *str, size_t len) {
  s->data = malloc(len);
  memcpy(s->data, str, len);
  s->size = len;
  s->capacity = len;
}

void string_clear(string *s) {
  s->size = 0;
}

void string_resize(string *s, size_t size) {
  if (size > s->capacity) {
    s->data = realloc(s->data, size);
    s->capacity = size;
  }
  s->size = size;
}

void string_reserve(string *s, size_t capacity) {
  if (capacity > s->capacity) {
    s->data = realloc(s->data, capacity);
    s->capacity = capacity;
  }
}

void string_expand_to_fit(string *s, size_t lower_bound) {
  if (s->size == 0) {
    string_reserve(s, lower_bound);
  } else {
    while (lower_bound > s->capacity) {
      string_reserve(s, 2 * s->capacity);
    }
  }
}

void string_append_c_str(string *s, const char *str, size_t len) {
  string_expand_to_fit(s, s->size + len);
  memcpy(s->data + s->size, str, len);
  s->size += len;
}

void string_append(string *s, const string *other) {
  string_append_c_str(s, other->data, other->size);
}

void string_append_char(string *s, char c, size_t len) {
  string_expand_to_fit(s, s->size + len);
  memset(s->data + s->size, c, len);
  s->size += len;
}

void string_insert_c_str(string *s, size_t pos, const char *str, size_t len) {
  string_expand_to_fit(s, s->size + len);
  memmove(s->data + pos + len, s->data + pos, s->size - pos);
  memcpy(s->data + pos, str, len);
  s->size += len;
}

// We need O(1) implementation for constant dst
void string_as_c_str(const string *s, char *dst) {
  memcpy(dst, s->data, s->size);
  dst[s->size] = '\0';
}

void string_insert_char(string *s, size_t pos, char c) {
  string_insert_c_str(s, pos, &c, 1); // This can be done in a more efficient manner :)
}

void string_erase(string *s, size_t pos, size_t len) {
  memmove(s->data + pos, s->data + pos + len, s->size - (len + pos));
  s->size -= len;
}
