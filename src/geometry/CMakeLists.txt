add_library(geometry point3d.h point3d.cpp vec3d.h vec3d.cpp)
target_include_directories(geometry INTERFACE ..)
target_link_libraries(geometry PRIVATE project_options project_warnings)
