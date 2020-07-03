add_library(rendering camera.cpp camera.h scene.cpp scene.h render.cpp render.h)
target_include_directories(rendering INTERFACE ..)
target_link_libraries(rendering project_options project_warnings geometry objects view)
