add_library(objects sphere.cpp sphere.h ray.cpp ray.h camera.cpp camera.h material.h scene.cpp scene.h triangle.cpp triangle.h)
target_include_directories(objects INTERFACE ..)
target_link_libraries(objects project_options project_warnings geometry)
