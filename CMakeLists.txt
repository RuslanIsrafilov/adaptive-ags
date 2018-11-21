cmake_minimum_required(VERSION 3.1.0)
project(adaptive_ags C CXX)
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

set(build_type "debug" CACHE STRING "Configuration: debug or release")
set(build_python "yes" CACHE STRING "Build python binding: yes or no")

if (build_type STREQUAL "release")
  set(CMAKE_BUILD_TYPE "Release")
elseif (build_type STREQUAL "debug")
  set(CMAKE_BUILD_TYPE "Debug")
else()
  message(FATAL_ERROR "Unknown configuration -> ${build_type}")
endif()
message(STATUS "Build in ${build_type} configuration")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(root_dir ${PROJECT_SOURCE_DIR})
set(src_dir  ${root_dir}/adaptive_ags)

add_subdirectory(${root_dir}/third_party/gtest)
add_subdirectory(${root_dir}/third_party/grishagins)
if (build_python STREQUAL "yes")
  add_subdirectory(${root_dir}/python)
endif()

# Adaptive AGS
file(GLOB_RECURSE src_files ${src_dir}/*.cpp)
set(src_test_files ${src_files})
list(FILTER src_files EXCLUDE REGEX ".+_test\\.cpp$")
list(FILTER src_test_files INCLUDE REGEX ".+_test\\.cpp$")

# Adaptive AGS library
add_library(adaptive_ags ${src_files})
target_include_directories(adaptive_ags PRIVATE ${root_dir})
set_target_properties(adaptive_ags
    PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    INTERFACE_INCLUDE_DIRECTORIES "${root_dir}"
)

# Adaptive AGS tests
add_executable(tests ${src_test_files} ${gtest_main_file})
target_include_directories(tests PRIVATE ${root_dir})
target_link_libraries(tests
  adaptive_ags
  grishagins
  gtest
  gtest_main
)
