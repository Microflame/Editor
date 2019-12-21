#ifndef STRING_H
#define STRING_H

#include <stdlib.h>

struct string {
  char *data;
  size_t size;
  size_t capacity;
};
typedef struct string string;

void string_free(string *s);

void string_init(string *s, size_t capacity);

void string_init_with_c_str(string *s, const char *str, size_t len);

void string_clear(string *s);

void string_resize(string *s, size_t size);

void string_reserve(string *s, size_t capacity);

void string_expand_to_fit(string *s, size_t lower_bound);

void string_append_c_str(string *s, const char *str, size_t len);

void string_append(string *s, const string *other);

void string_append_char(string *s, char c, size_t len);

void string_insert_c_str(string *s, size_t pos, const char *str, size_t len);

void string_as_c_str(const string *s, char *dst);

void string_insert_char(string *s, size_t pos, char c);

void string_erase(string *s, size_t pos, size_t len);

#endif /* end of include guard: STRING_H */
