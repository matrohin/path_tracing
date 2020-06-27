add_executable(path_tracing main.cpp)

add_subdirectory(geometry)
add_subdirectory(objects)

target_link_libraries(path_tracing PRIVATE project_options project_warnings geometry objects ${CONAN_LIBS})
