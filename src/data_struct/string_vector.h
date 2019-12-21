#ifndef STRING_VECTOR_H
#define STRING_VECTOR_H

#include <data_struct/string.h>

struct string_vector {
  string *data;
  size_t size;
  size_t capacity;
};
typedef struct string_vector string_vector;

void string_vector_reserve(string_vector *sv, size_t capacity);

void string_vector_init(string_vector *sv, size_t capacity);

string* string_vector_append_c_str(string_vector *sv, const char *str, size_t len);

string* string_vector_append(string_vector *sv, size_t len);

string* string_vector_insert_c_str(string_vector *sv, size_t pos, const char *str, size_t len);

string* string_vector_insert(string_vector *sv, size_t pos, size_t capacity);

void string_vector_pop(string_vector *sv, size_t pos);

void string_vector_join(const string_vector *sv, string *dst, const string *on);

void string_vector_clear(string_vector *sv);

void string_vector_free(string_vector *sv);

#endif /* end of include guard: STRING_VECTOR_H */
