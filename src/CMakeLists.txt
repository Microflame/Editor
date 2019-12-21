add_executable(editor main.c)
add_library(status_bar editor/status_bar.c)
add_library(data_struct data_struct/string.c data_struct/string_vector.c)
target_link_libraries(editor status_bar data_struct)

include_directories(.)
