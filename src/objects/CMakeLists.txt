add_library(objects sphere.cpp sphere.h triangle.cpp triangle.h material.h ray.cpp ray.h)
target_include_directories(objects INTERFACE ..)
target_link_libraries(objects project_options project_warnings geometry)
