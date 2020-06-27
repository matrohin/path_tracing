add_library(view view.cpp view.h color.cpp color.h)
target_include_directories(view INTERFACE ..)
target_link_libraries(view project_options project_warnings)
