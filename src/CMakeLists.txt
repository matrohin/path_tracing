add_executable(path_tracing main.cpp)

add_subdirectory(geometry)
add_subdirectory(images)
add_subdirectory(objects)
add_subdirectory(rendering)
add_subdirectory(view)

find_package(Threads REQUIRED)

target_link_libraries(path_tracing PRIVATE project_options project_warnings
  geometry rendering objects view images Threads::Threads ${CONAN_LIBS})
