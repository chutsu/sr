#!/bin/bash
set -e

check() {
	valgrind --leak-check=full $1 $2 $3
}

make
# check_mem ./build/tests/test_sr
# ./build/tests/test_sr --target test_stack_setup
# ./build/tests/test_sr --target test_stack_push
# ./build/tests/test_sr --target test_stack_pop
# ./build/tests/test_sr --target test_node_new_and_delete
# ./build/tests/test_sr --target test_node_copy
# ./build/tests/test_sr --target test_node_string
# ./build/tests/test_sr --target test_node_new_func
# ./build/tests/test_sr --target test_node_new_input
# ./build/tests/test_sr --target test_node_new_const
# ./build/tests/test_sr --target test_random_func
# ./build/tests/test_sr --target test_random_term
# ./build/tests/test_sr --target test_tree_new_and_delete
# ./build/tests/test_sr --target test_tree_copy
# ./build/tests/test_sr --target test_tree_string
# ./build/tests/test_sr --target test_tree_generate
# ./build/tests/test_sr --target test_tree_update
# ./build/tests/test_sr --target test_tree_get_node
# ./build/tests/test_sr --target test_tree_select_rand_func
# ./build/tests/test_sr --target test_tree_stack
# ./build/tests/test_sr --target test_subtree_size
# ./build/tests/test_sr --target test_point_mutation
# ./build/tests/test_sr --target test_point_crossover
# ./build/tests/test_sr --target test_tournament_selection
# ./build/tests/test_sr --target test_csv_rows
# ./build/tests/test_sr --target test_csv_cols
# ./build/tests/test_sr --target test_csv_fields
# ./build/tests/test_sr --target test_csv_data
# ./build/tests/test_sr --target test_dataset_load_and_delete
# ./build/tests/test_sr --target test_best_tree
# ./build/tests/test_sr --target test_evaluate_tree
# ./build/tests/test_sr --target test_regress
# ./build/tests/test_sr --target test_regress

# python sr/sr.py TestCrossover.test_point_crossover
# python sr/sr.py TestRegression.test_eval_tree
# python sr/sr.py TestRegression.test_regress
