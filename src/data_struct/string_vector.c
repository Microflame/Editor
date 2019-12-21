#include <data_struct/string_vector.h>

#include <string.h>

void string_vector_reserve(string_vector *sv, size_t capacity) {
  if (capacity > sv->capacity) {
    sv->data = realloc(sv->data, capacity * sizeof(string));
    sv->capacity = capacity;
  }
}

void string_vector_init(string_vector *sv, size_t capacity) {
  sv->data = malloc(capacity * sizeof(string));
  sv->capacity = capacity;
  sv->size = 0;
}

string* string_vector_append_c_str(string_vector *sv, const char *str, size_t len) {
  if (sv->size == sv->capacity) {
    string_vector_reserve(sv, 2 * sv->capacity);
  }
  string_init_with_c_str(&(sv->data[sv->size]), str, len);
  sv->size++;
  return &(sv->data[sv->size - 1]);
}

string* string_vector_append(string_vector *sv, size_t len) {
  if (sv->size == sv->capacity) {
    string_vector_reserve(sv, 2 * sv->capacity);
  }
  string_init(&(sv->data[sv->size]), len);
  sv->size++;
  return &(sv->data[sv->size - 1]);
}

string* string_vector_insert_c_str(string_vector *sv, size_t pos, const char *str, size_t len) {
  if (sv->size == sv->capacity) {
    string_vector_reserve(sv, 2 * sv->capacity);
  }
  memmove(sv->data + pos + 1, sv->data + pos, (sv->size - pos) * sizeof(string));
  string_init_with_c_str(&(sv->data[pos]), str, len);
  sv->size++;
  return &(sv->data[pos]);
}

string* string_vector_insert(string_vector *sv, size_t pos, size_t capacity) {
  if (sv->size == sv->capacity) {
    string_vector_reserve(sv, 2 * sv->capacity);
  }
  memmove(sv->data + pos + 1, sv->data + pos, (sv->size - pos) * sizeof(string));
  string_init(sv->data + pos, capacity);
  sv->size++;
  return &(sv->data[pos]);
}

void string_vector_pop(string_vector *sv, size_t pos) {
  memmove(sv->data + pos, sv->data + pos + 1, (sv->size - pos - 1) * sizeof(string));
  sv->size--;
}

void string_vector_join(const string_vector *sv, string *dst, const string *on) {
  string_clear(dst);
  size_t i;
  for (i = 0; i < sv->size - 1; ++i) {
    string_append(dst, &(sv->data[i]));
    string_append(dst, on);
  }
  string_append(dst, &(sv->data[sv->size - 1]));
}

void string_vector_clear(string_vector *sv) {
  size_t i;
  for (i = 0; i < sv->size; ++i) {
    string_free(&(sv->data[i]));
  }
  sv->size = 0;
}

void string_vector_free(string_vector *sv) {
  string_vector_clear(sv);
  free(sv->data);
}
