set(python_dir ${CMAKE_CURRENT_SOURCE_DIR})
set(src_python_dir ${python_dir}/adaptive_ags)
set(adaptive_ags_pxd ${src_python_dir}/adaptive_ags_.pxd)
set(adaptive_ags_pyx ${src_python_dir}/adaptive_ags_.pyx)
set(adaptive_ags_cpp ${CMAKE_BINARY_DIR}/adaptive_ags_cython.cpp)
set(output_dir ${CMAKE_BINARY_DIR}/bin/python)

find_package(PythonInterp 3.4 REQUIRED)
find_package(PythonLibs 3.4 REQUIRED)
find_package(NumPy REQUIRED)

# Compile Cython-generated code to native Python extension
add_library(adaptive_ags_ SHARED ${adaptive_ags_cpp})
set_target_properties(adaptive_ags_
  PROPERTIES
  PREFIX ""
  SUFFIX ".so"
  LIBRARY_OUTPUT_DIRECTORY "${output_dir}/adaptive_ags"
)
target_include_directories(adaptive_ags_ PRIVATE ${PYTHON_INCLUDE_DIRS}
                                                 ${PYTHON_NUMPY_INCLUDE_DIR})
target_link_libraries(adaptive_ags_
  adaptive_ags
  grishagins
  ${PYTHON_LIBRARIES}
)

# Compile Cython to C++
add_custom_command(
  OUTPUT ${adaptive_ags_cpp}
  COMMAND ${PYTHON_EXECUTABLE} -m cython -3 --cplus -o ${adaptive_ags_cpp}
                               -I${src_python_dir} ${adaptive_ags_pyx}
  DEPENDS ${adaptive_ags_pxd} ${adaptive_ags_pyx}
)
add_custom_target(adaptive_ags_cython_pyx DEPENDS ${adaptive_ags_cpp})
add_dependencies(adaptive_ags_ adaptive_ags_cython_pyx)

# Replciate python source tree to destination directory
file(GLOB_RECURSE template_py_files RELATIVE ${python_dir} ${python_dir}/*.py)
foreach (template_file ${template_py_files})
  set(src_template_path ${python_dir}/${template_file})
  set(dst_template_path ${output_dir}/${template_file})
  if (NOT IS_DIRECTORY ${src_template_path})
    list(APPEND dst_py_files "${dst_template_path}")
    add_custom_command(
      OUTPUT ${dst_template_path}
      COMMAND ${CMAKE_COMMAND} -E copy
              ${src_template_path}
              ${dst_template_path}
      DEPENDS ${src_template_path}
      COMMENT "Copying ${template_file} to ./bin/python"
    )
  endif()
endforeach()
add_custom_target(adaptive_ags_py DEPENDS ${dst_py_files})
add_dependencies(adaptive_ags_ adaptive_ags_py)

add_custom_target(install_py
  COMMAND ${PYTHON_EXECUTABLE} setup.py install
  WORKING_DIRECTORY ${output_dir}
)
add_dependencies(install_py adaptive_ags_)
