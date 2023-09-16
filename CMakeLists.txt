cmake_minimum_required (VERSION 3.8 FATAL_ERROR)

find_program (CLANG_TIDY_EXE NAMES "clang-tidy")
if (CLANG_TIDY_EXE)
  set (CMAKE_CXX_CLANG_TIDY clang-tidy)
endif ()

set (CMAKE_CXX_STANDARD 17)

project (samples)

include_directories (${PROJECT_SOURCE_DIR}/src)

set (TEST_LIB_SOURCES
  ${PROJECT_SOURCE_DIR}/src/test/TestCaseContainer.cc)
add_library (test_lib STATIC ${TEST_LIB_SOURCES})

set (SIMPLE_OPTIMIZER_SOURCES
  ${PROJECT_SOURCE_DIR}/src/compiler_structs.cc
  ${PROJECT_SOURCE_DIR}/src/constant_fold.cc
  ${PROJECT_SOURCE_DIR}/src/constant_prop.cc
  ${PROJECT_SOURCE_DIR}/src/dead_code.cc
  ${PROJECT_SOURCE_DIR}/src/simple_optimizer.cc)
add_executable (simple_optimizer ${SIMPLE_OPTIMIZER_SOURCES})

set (ARRAY_STUFF_SOURCES
  ${PROJECT_SOURCE_DIR}/src/test/TestCaseContainer.cc
  ${PROJECT_SOURCE_DIR}/src/array_combinations.cc
  ${PROJECT_SOURCE_DIR}/src/array_gas_stations.cc
  ${PROJECT_SOURCE_DIR}/src/array_h_index.cc
  ${PROJECT_SOURCE_DIR}/src/array_jump_game.cc
  ${PROJECT_SOURCE_DIR}/src/array_shift.cc
  ${PROJECT_SOURCE_DIR}/src/array_majority_element.cc
  ${PROJECT_SOURCE_DIR}/src/array_max_delta.cc
  ${PROJECT_SOURCE_DIR}/src/array_maze.cc
  ${PROJECT_SOURCE_DIR}/src/array_min_rewards.cc
  ${PROJECT_SOURCE_DIR}/src/array_rain.cc
  ${PROJECT_SOURCE_DIR}/src/array_remove_duplicates.cc
  ${PROJECT_SOURCE_DIR}/src/array_remove_element.cc
  ${PROJECT_SOURCE_DIR}/src/array_search.cc
  ${PROJECT_SOURCE_DIR}/src/array_sorted_merge.cc
  ${PROJECT_SOURCE_DIR}/src/array_stuff.cc
  ${PROJECT_SOURCE_DIR}/src/array_sudoku.cc
  ${PROJECT_SOURCE_DIR}/src/array_two_sum.cc
  ${PROJECT_SOURCE_DIR}/src/bitwise_ops.cc
  ${PROJECT_SOURCE_DIR}/src/get_sum_node_network.cc
  ${PROJECT_SOURCE_DIR}/src/graph_islands_count.cc
  ${PROJECT_SOURCE_DIR}/src/graph_islands_identify.cc
  ${PROJECT_SOURCE_DIR}/src/graph_islands_size.cc
  ${PROJECT_SOURCE_DIR}/src/hashmap_cache.cc
  ${PROJECT_SOURCE_DIR}/src/hashmap_duplicates.cc
  ${PROJECT_SOURCE_DIR}/src/hashmap_happy_number.cc
  ${PROJECT_SOURCE_DIR}/src/hashmap_ransom.cc
  ${PROJECT_SOURCE_DIR}/src/hashmap_redis.cc
  ${PROJECT_SOURCE_DIR}/src/hashmap_sequence.cc
  ${PROJECT_SOURCE_DIR}/src/hashmap_strings.cc
  ${PROJECT_SOURCE_DIR}/src/math_make_change.cc
  ${PROJECT_SOURCE_DIR}/src/random_select.cc
  ${PROJECT_SOURCE_DIR}/src/string_find_word_under_cursor.cc
  ${PROJECT_SOURCE_DIR}/src/test_islands.cc
  ${PROJECT_SOURCE_DIR}/src/udp_buffer.cc)
add_executable (array_stuff ${ARRAY_STUFF_SOURCES})
target_link_libraries (array_stuff test_lib)

if (MSVC)
  # disable some benign warnings on MSVC
  add_compile_options ("/Wall;/wd4514;/wd4625;/wd4626;/wd5026;/wd5027;/wd5045;/wd4710;/wd4820;")
else ()
  add_compile_options ("-Wall;")
endif ()
