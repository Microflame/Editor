#ifndef FILE_IO_H
#define FILE_IO_H

#define _BSD_SOURCE
#define _GNU_SOURCE

#include <stdio.h>
#include <fcntl.h>

#include <data_struct/string_vector.h>

int read_file(const char *fname, string_vector *sv) {
  FILE *f = fopen(fname, "r");
  if (!f) die("fopen");
  string_vector_clear(sv);

  char *line = NULL;
  int lines_read = 0;
  size_t linecap = 0;
  int linelen;
  linelen = getline(&line, &linecap, f);
  while (linelen != -1) {
    while (linelen > 0 && (line[linelen - 1] == '\r' ||
    line[linelen - 1] == '\n')) {
      linelen--;
    }
    string_vector_append_c_str(sv, line, linelen);

    linelen = getline(&line, &linecap, f);
    lines_read += 1;
  }
  string_vector_append(sv, 1);
  free(line);
  fclose(f);

  return lines_read;
}

int write_file(const char *fname, const string *joined) {
  int fd = open(fname, O_RDWR | O_CREAT);
  ftruncate(fd, joined->size);
  write(fd, joined->data, joined->size);
  close(fd);
  return 1;
}

#endif /* end of include guard: FILE_IO_H */
