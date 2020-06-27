add_executable(path_tracing main.cpp)

add_subdirectory(geometry)
add_subdirectory(objects)
add_subdirectory(images)
add_subdirectory(view)

target_link_libraries(path_tracing PRIVATE project_options project_warnings geometry objects view images ${CONAN_LIBS})
