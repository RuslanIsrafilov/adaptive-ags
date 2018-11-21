set(GTEST_INC ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(GTEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_library(gtest ${GTEST_DIR}/gtest-all.cc)
target_link_libraries(gtest pthread)

target_include_directories(gtest
  PRIVATE
  "${GTEST_INC}"
  "${CMAKE_CURRENT_SOURCE_DIR}"
)

add_library(gtest_main ${GTEST_DIR}/gtest_main.cc)

target_include_directories(gtest_main
  PRIVATE
  "${GTEST_INC}"
  "${CMAKE_CURRENT_SOURCE_DIR}"
)

set_target_properties(gtest
  PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${GTEST_INC}"
  POSITION_INDEPENDENT_CODE ON
)
