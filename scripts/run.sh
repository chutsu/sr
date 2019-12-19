#!/bin/bash
set -e

# make
# ./build/tests/test_sr --target test_node_setup
# ./build/tests/test_sr --target test_node_clear
# ./build/tests/test_sr --target test_node_setup_func
# ./build/tests/test_sr --target test_node_setup_input
# ./build/tests/test_sr --target test_node_setup_const
# ./build/tests/test_sr --target test_random_func
# ./build/tests/test_sr --target test_random_term
# ./build/tests/test_sr --target test_tree_setup
# ./build/tests/test_sr --target test_tree_clear
# ./build/tests/test_sr --target test_tree_generate
# ./build/tests/test_sr --target test_tree_update
# ./build/tests/test_sr --target test_tree_get_node
# ./build/tests/test_sr --target test_tree_select_rand_func
# ./build/tests/test_sr --target test_subtree_size
# ./build/tests/test_sr --target test_point_mutation
# ./build/tests/test_sr --target test_point_crossover

# cd sr && node sr.js

python sr/sr.py
