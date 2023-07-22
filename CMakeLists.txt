cmake_minimum_required (VERSION 3.8 FATAL_ERROR)

find_program (CLANG_TIDY_EXE NAMES "clang-tidy")
if (CLANG_TIDY_EXE)
  set (CMAKE_CXX_CLANG_TIDY clang-tidy)
endif ()

set (CMAKE_CXX_STANDARD 17)

project (samples)

include_directories (${PROJECT_SOURCE_DIR}/src)

set (SIMPLE_OPTIMIZER_SOURCES ${PROJECT_SOURCE_DIR}/src/compiler_structs.cc
  ${PROJECT_SOURCE_DIR}/src/constant_fold.cc
  ${PROJECT_SOURCE_DIR}/src/constant_prop.cc
  ${PROJECT_SOURCE_DIR}/src/dead_code.cc
  ${PROJECT_SOURCE_DIR}/src/simple_optimizer.cc)
add_executable (simple_optimizer ${SIMPLE_OPTIMIZER_SOURCES})

set (NODE_GET_SUM_SOURCES ${PROJECT_SOURCE_DIR}/src/get_sum_node_network.cc)
add_executable (node_get_sum ${NODE_GET_SUM_SOURCES})

set (UDP_BUFFER_SOURCES ${PROJECT_SOURCE_DIR}/src/udp_buffer.cc)
add_executable (udp_buffer ${UDP_BUFFER_SOURCES})

if (MSVC)
  # disable some benign warnings on MSVC
  add_compile_options ("/Wall;/wd4514;/wd4625;/wd4626;/wd5026;/wd5027;/wd5045;/wd4710;/wd4820;")
else ()
  add_compile_options ("-Wall;")
endif ()
