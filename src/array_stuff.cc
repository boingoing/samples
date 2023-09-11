#include "array_combinations.h"
#include "array_gas_stations.h"
#include "array_h_index.h"
#include "array_jump_game.h"
#include "array_majority_element.h"
#include "array_max_delta.h"
#include "array_maze.h"
#include "array_min_rewards.h"
#include "array_rain.h"
#include "array_remove_duplicates.h"
#include "array_remove_element.h"
#include "array_search.h"
#include "array_shift.h"
#include "array_sorted_merge.h"
#include "array_sudoku.h"
#include "array_two_sum.h"
#include "bitwise_ops.h"
#include "graph_islands_count.h"
#include "graph_islands_identify.h"
#include "graph_islands_size.h"
#include "hashmap_happy_number.h"
#include "hashmap_ransom.h"
#include "hashmap_redis.h"

#include "test_helpers.h"

int main() {
  main_merge();
  main_remove();
  main_remove_duplicates();
  main_array_majority_element();
  main_array_shift();
  main_max_delta();
  main_jump_game();
  main_h_index();
  main_gas_stations();
  main_sudoku();
  main_min_rewards();
  main_rain();
  main_ransom();
  main_two_sum();
  main_happy_number();
  main_array_search();
  main_maze();
  main_redis();
  main_combinations();
  main_bitwise_ops();
  main_identify_islands();
  main_count_islands();
  main_largest_island();

  TestCaseContainer::runAllTests();

  return 0;
}
