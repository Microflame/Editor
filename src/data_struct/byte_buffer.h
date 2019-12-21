#ifndef BYTE_BUFFER_H
#define BYTE_BUFFER_H

#include <string.h>

struct byte_buffer {
  unsigned char *buffer;
  size_t capacity;
  size_t limit;
  size_t position;
};
typedef struct byte_buffer byte_buffer;

void byte_buffer_clear(byte_buffer *bb) {
  bb->position = 0;
  bb->limit = bb->capacity;
}

void byte_buffer_flip(byte_buffer *bb) {
  bb->limit = bb->position;
  bb->position = 0;
}

void byte_buffer_compact(byte_buffer *bb) {
  size_t remaining = bb->limit - bb->position;
  memmove(bb->buffer, bb->buffer + bb->position, remaining);
  bb->position = remaining;
  bb->limit = bb->capacity;
}

void byte_buffer_init(byte_buffer *bb, size_t size) {
  bb->buffer = malloc(size);
  bb->capacity = size;
  byte_buffer_clear(bb);
}

#endif /* end of include guard: BYTE_BUFFER_H */
