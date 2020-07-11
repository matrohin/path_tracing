add_library(parsing objloader.cpp objloader.h camloader.cpp camloader.h error.h parsing_utils.h)
target_include_directories(parsing INTERFACE ..)
target_link_libraries(parsing project_options project_warnings rendering view geometry objects)

if (NOT MSVC)
  target_link_libraries(parsing stdc++fs)
endif()
