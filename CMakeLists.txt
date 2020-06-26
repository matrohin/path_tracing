cmake_minimum_required(VERSION 3.10)

project(path_tracing VERSION 0.0.1 LANGUAGES CXX)

# Link this 'library' to set the c++ standard / compile-time options requested
add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_17)

# Link this 'library' to use the standard warnings
add_library(project_warnings INTERFACE)
if (MSVC)
  target_compile_options(project_warnings INTERFACE /W4 "/permissive-")
else()
  target_compile_options(project_warnings
    INTERFACE
      -Wall
      -Werror # treat all warnings as errors
      -Wextra # reasonable and standard
      -Wshadow # warn the user if a variable declaration shadows one from a parent context
      -Wnon-virtual-dtor # warn the user if a class with virtual functions has a non-virtual destructor.
      # This helps catch hard to track down memory errors
      -Wcast-align # warn for potential performance problem casts
      -Wunused # warn on anything being unused
      -Woverloaded-virtual # warn if you overload (not override) a virtual function
      -Wpedantic # warn if non-standard C++ is used
      -Wnull-dereference # warn if a null dereference is detected
      -Wdouble-promotion # warn if float is implicit promoted to double
      -Wformat=2 # warn on security issues around functions that format output (ie printf)
      )
endif()

# Conan setup
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_subdirectory(src)

