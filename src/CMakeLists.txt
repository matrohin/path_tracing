add_executable(path_tracing main.cpp)
target_link_libraries(path_tracing PRIVATE project_options project_warnings ${CONAN_LIBS})
