add_library(images png_utils.cpp png_utils.h)
target_include_directories(images INTERFACE ..)
target_link_libraries(images project_options project_warnings view)
