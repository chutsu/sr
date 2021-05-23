#!/bin/bash
set -e

debug() {
  gdb \
    -ex=run \
    -ex=bt \
    -ex="set confirm off" \
    -ex=quit \
    --args "$1" "$2" "$3"
}

check() {
	valgrind --leak-check=full $1 $2 $3
}

make clean
time make
# check ./build/test_sr
# ./build/test_sr --target test_randi
# ./build/test_sr --target test_randf
# ./build/test_sr --target test_malloc_string
# ./build/test_sr --target test_stack_setup
# ./build/test_sr --target test_stack_push
# ./build/test_sr --target test_stack_pop
# ./build/test_sr --target test_node_new_and_delete
# ./build/test_sr --target test_node_copy
# ./build/test_sr --target test_node_string
# ./build/test_sr --target test_node_new_func
# ./build/test_sr --target test_node_new_input
# ./build/test_sr --target test_node_new_const
# ./build/test_sr --target test_random_func
# ./build/test_sr --target test_random_term
# ./build/test_sr --target test_tree_new_and_delete
# ./build/test_sr --target test_tree_copy
# ./build/test_sr --target test_tree_string
# ./build/test_sr --target test_tree_generate
# ./build/test_sr --target test_tree_update
# ./build/test_sr --target test_tree_get_node
# ./build/test_sr --target test_tree_select_rand_func
# ./build/test_sr --target test_tree_stack
# ./build/test_sr --target test_subtree_size
# ./build/test_sr --target test_point_mutation
# debug ./build/test_sr --target test_subtree_mutation
# ./build/test_sr --target test_point_crossover
# ./build/test_sr --target test_tournament_selection
# ./build/test_sr --target test_csv_rows
# ./build/test_sr --target test_csv_cols
# ./build/test_sr --target test_csv_fields
# ./build/test_sr --target test_csv_data
# ./build/test_sr --target test_dataset_load_and_delete
# ./build/test_sr --target test_best_tree
# ./build/test_sr --target test_evaluate_tree
# debug ./build/test_sr --target test_regress

# python sr/sr.py TestCrossover.test_point_crossover
# python sr/sr.py TestRegression.test_eval_tree
# python sr/sr.py TestRegression.test_regress
# python sr/sr.py TestAprilGrid.test_constructor
# python sr/sr.py TestAprilGrid.test_load
# python sr/sr.py TestDataset.test_load_calib_data
