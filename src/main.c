#include <unistd.h>

#include <utils/termutils.h>
#include <utils/debug.h>
#include <data_struct/byte_buffer.h>
#include <editor/editor.h>

const int BUFFER_SIZE = 2048;

int main(int argc, const char *argv[]) {
  if (argc != 2) {
    printf("Usage: %s <fname>\n", argv[0]);
    return 0;
  }
  terminal_init();
  terminal_enable_raw_mode();
  editor_init(argv[1]);

  byte_buffer bb;
  byte_buffer_init(&bb, BUFFER_SIZE); // the buffer is alive until exit()
  int alive = 1;
  int num_read;
  while (alive) {
    if ((num_read = terminal_read(STDIN_FILENO, bb.buffer + bb.position,
      bb.limit - bb.position, 1.0)) > 0) {
      bb.position += num_read;
      byte_buffer_flip(&bb);
      num_read = handle_bytes(bb.buffer, bb.limit);
      if (num_read == -1) {
        alive = 0;
      } else {
        bb.position += num_read;
        byte_buffer_compact(&bb);
      }
    } else {
      if (errno == EAGAIN) {
      } else if (num_read != 0) {
        die("read");
      }
    }
  }

  return 0;
}
