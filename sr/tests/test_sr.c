#include <string.h>

#include "sr/munit.h"
#include "sr/sr.h"

/******************************************************************************
 *                                   NODE
 ******************************************************************************/

int test_node_setup() {
  node_t n;
  node_setup(&n);

  /* General */
  MU_CHECK(n.type == -1);
  MU_CHECK(n.parent == NULL);
  MU_CHECK(n.nth_child == -1);

  /* Terminal specific */
  MU_CHECK(fltcmp(n.value, 0.0) == 0);

  /* Function specific */
  MU_CHECK(n.function == -1);
  MU_CHECK(n.arity == -1);
  for (int i = 0; i < MAX_ARITY; i++) {
    MU_CHECK(n.children[i] == NULL);
  }

  /* Evaluation */
  MU_CHECK(fltcmp(n.error, 0.0) == 0);

  return 0;
}

int test_node_new_and_delete() {
  return 0;
}

int test_node_clear() {
  node_t n;
  node_clear(&n);

  /* General */
  MU_CHECK(n.type == -1);
  MU_CHECK(n.parent == NULL);
  MU_CHECK(n.nth_child == -1);

  /* Terminal specific */
  MU_CHECK(fltcmp(n.value, 0.0) == 0);

  /* Function specific */
  MU_CHECK(n.function == -1);
  MU_CHECK(n.arity == -1);
  for (int i = 0; i < MAX_ARITY; i++) {
    MU_CHECK(n.children[i] == NULL);
  }

  /* Evaluation */
  MU_CHECK(fltcmp(n.error, 0.0) == 0);

  return 0;
}

int test_node_setup_func() {
  node_t n;
  node_setup_func(&n, ADD, ADD_ARITY);

  MU_CHECK(n.type == FUNC_NODE);
  MU_CHECK(n.function == ADD);
  MU_CHECK(n.arity == ADD_ARITY);

  return 0;
}

int test_node_setup_input() {
  node_t n;
  node_setup_input(&n, "x");

  MU_CHECK(n.type == TERM_NODE);
  MU_CHECK(n.data_type == INPUT);
  MU_CHECK(strcmp(n.input_name, "x") == 0);

  return 0;
}

int test_node_setup_const() {
  node_t n;
  node_setup_const(&n, 1.0);

  MU_CHECK(n.type == TERM_NODE);
  MU_CHECK(n.data_type == CONST);
  MU_CHECK(fltcmp(n.value, 1.0) == 0);

  return 0;
}

int test_random_func() {
  /* Setup function set */
  function_set_t fs;
  fs.funcs = (int[7]) {ADD, SUB, MUL, DIV, POW, EXP, LOG};
  fs.arity = (int[7]) {ADD_ARITY, SUB_ARITY, MUL_ARITY, DIV_ARITY,
                       POW_ARITY, EXP_ARITY, LOG_ARITY};
  fs.length = 7;
  function_set_print(&fs);

  /* Generate random function node */
  node_t n;
  random_func(&fs, &n);

  /* Assert */
  node_print(&n);
  MU_CHECK(n.type == FUNC_NODE);
  MU_CHECK(n.function >= 0 || n.function <= 6);
  MU_CHECK(n.arity >= 0 || n.arity <= 2);

  return 0;
}

int test_random_term() {
  /* Setup terminal set */
  terminal_set_t ts;
  ts.terms = (terminal_t[13]) {
    {.type = CONST, .val = 0.0},
    {.type = CONST, .val = 1.0},
    {.type = CONST, .val = 2.0},
    {.type = CONST, .val = 3.0},
    {.type = CONST, .val = 4.0},
    {.type = CONST, .val = 5.0},
    {.type = CONST, .val = 6.0},
    {.type = CONST, .val = 7.0},
    {.type = CONST, .val = 8.0},
    {.type = CONST, .val = 9.0},
    {.type = INPUT, .str = "x"},
    {.type = INPUT, .str = "y"},
    {.type = RCONST, .range = {-1.0, 1.0}},
  };
  ts.length = 13;
  terminal_set_print(&ts);

  /* Generate random terminal node */
  node_t n;
  random_term(&ts, &n);

  /* Assert */
  node_print(&n);
  MU_CHECK(n.type == TERM_NODE);

  return 0;
}

/******************************************************************************
 *                                   TREE
 ******************************************************************************/

int test_tree_setup() {
  tree_t t;
  tree_setup(&t);

  MU_CHECK(t.root == NULL);
  MU_CHECK(t.size == 0);
  MU_CHECK(t.depth == 0);

  MU_CHECK(fltcmp(t.error, 0.0) == 0.0);
  MU_CHECK(fltcmp(t.score, 0.0) == 0.0);
  MU_CHECK(fltcmp(t.hits, 0.0) == 0.0);

  return 0;
}

int test_tree_clear() {
  tree_t t;
  tree_setup(&t);
  tree_clear(&t);

  MU_CHECK(t.root == NULL);
  MU_CHECK(t.size == 0);
  MU_CHECK(t.depth == 0);

  MU_CHECK(fltcmp(t.error, 0.0) == 0.0);
  MU_CHECK(fltcmp(t.score, 0.0) == 0.0);
  MU_CHECK(fltcmp(t.hits, 0.0) == 0.0);

  return 0;
}

int test_tree_generate() {
  /* Setup function set */
  function_set_t fs;
  fs.funcs = (int[7]) {ADD, SUB, MUL, DIV, POW, EXP, LOG};
  fs.arity = (int[7]) {ADD_ARITY, SUB_ARITY, MUL_ARITY, DIV_ARITY,
                       POW_ARITY, EXP_ARITY, LOG_ARITY};
  fs.length = 7;
  function_set_print(&fs);

  /* Setup terminal set */
  terminal_set_t ts;
  ts.terms = (terminal_t[13]) {
    {.type = CONST, .val = 0.0},
    {.type = CONST, .val = 1.0},
    {.type = CONST, .val = 2.0},
    {.type = CONST, .val = 3.0},
    {.type = CONST, .val = 4.0},
    {.type = CONST, .val = 5.0},
    {.type = CONST, .val = 6.0},
    {.type = CONST, .val = 7.0},
    {.type = CONST, .val = 8.0},
    {.type = CONST, .val = 9.0},
    {.type = INPUT, .str = "x"},
    {.type = INPUT, .str = "y"},
    {.type = RCONST, .range = {-1.0, 1.0}},
  };
  ts.length = 13;
  terminal_set_print(&ts);

  /* Generate tree */
  tree_t t;
  int retval = tree_generate(FULL, &fs, &ts, 2, &t);
  MU_CHECK(retval == 0);

  /* Print tree */
  tree_print(&t);
  tree_clear(&t);
  /* MU_CHECK(t.nodes[t.size + 1].type == -1); */

  return 0;
}

int test_tree_update() {
  /* Setup */
  tree_t t;
  tree_setup(&t);

  /* Root */
  node_t *root = node_new();
  node_setup_func(root, ADD, 2);
  t.root = root;

  /* Depth 1 - Left */
  node_t *sub = node_new();
  node_t *k1 = node_new();
  node_t *k2 = node_new();

  node_setup_func(sub, SUB, 2);
  node_setup_const(k1, 1.0);
  node_setup_const(k2, 2.0);

  root->children[0] = sub;
  sub->children[0] = k1;
  sub->children[1] = k2;

  /* Depth 1 - Right */
  node_t *mul = node_new();
  node_t *k3 = node_new();
  node_t *k4 = node_new();
  node_setup_func(mul, MUL, 2);
  node_setup_const(k3, 3.0);
  node_setup_const(k4, 4.0);

  root->children[1] = mul;
  mul->children[0] = k3;
  mul->children[1] = k4;

  tree_print(&t);
  tree_update(&t);

  return 0;
}

int test_tree_get_node() {
  /* Setup */
  tree_t t;
  tree_setup(&t);

  /* Root */
  node_t *root = node_new();
  node_setup_func(root, ADD, 2);
  t.root = root;

  /* Left */
  node_t *sub = node_new();
  node_t *k1 = node_new();
  node_t *k2 = node_new();

  node_setup_func(sub, SUB, 2);
  node_setup_const(k1, 1.0);
  node_setup_const(k2, 2.0);

  root->children[0] = sub;
  sub->children[0] = k1;
  sub->children[1] = k2;

  /* Right */
  node_t *mul = node_new();
  node_t *k3 = node_new();
  node_t *k4 = node_new();
  node_setup_func(mul, MUL, 2);
  node_setup_const(k3, 3.0);
  node_setup_const(k4, 4.0);

  root->children[1] = mul;
  mul->children[0] = k3;
  mul->children[1] = k4;

  /* Assert */
  tree_print(&t);
  {
    node_t *n = tree_get_node(&t, 0);
    MU_CHECK(n->type == FUNC_NODE);
    MU_CHECK(n->function == ADD);
    MU_CHECK(n->arity == 2);
  }

  {
    node_t *n = tree_get_node(&t, 1);
    MU_CHECK(n->type == FUNC_NODE);
    MU_CHECK(n->function == SUB);
    MU_CHECK(n->arity == 2);
  }

  {
    node_t *n = tree_get_node(&t, 2);
    MU_CHECK(n->type == TERM_NODE);
    MU_CHECK(n->data_type == CONST);
    MU_CHECK(fltcmp(n->value, 1.0) == 0);
  }

  {
    node_t *n = tree_get_node(&t, 3);
    MU_CHECK(n->type == TERM_NODE);
    MU_CHECK(n->data_type == CONST);
    MU_CHECK(fltcmp(n->value, 2.0) == 0);
  }

  {
    node_t *n = tree_get_node(&t, 4);
    MU_CHECK(n->type == FUNC_NODE);
    MU_CHECK(n->function == MUL);
    MU_CHECK(n->arity == 2);
  }

  {
    node_t *n = tree_get_node(&t, 5);
    MU_CHECK(n->type == TERM_NODE);
    MU_CHECK(n->data_type == CONST);
    MU_CHECK(fltcmp(n->value, 3.0) == 0);
  }

  {
    node_t *n = tree_get_node(&t, 6);
    MU_CHECK(n->type == TERM_NODE);
    MU_CHECK(n->data_type == CONST);
    MU_CHECK(fltcmp(n->value, 4.0) == 0);
  }

  {
    node_t *n = tree_get_node(&t, 9);
    MU_CHECK(n == NULL);
  }

  /* Clean up */
  tree_clear(&t);

  return 0;
}

int test_tree_select_rand_func() {
  /* Setup */
  tree_t t;
  tree_setup(&t);

  /* Root */
  node_t *root = node_new();
  node_setup_func(root, ADD, 2);
  t.root = root;

  /* Left */
  node_t *sub = node_new();
  node_t *k1 = node_new();
  node_t *k2 = node_new();

  node_setup_func(sub, SUB, 2);
  node_setup_const(k1, 1.0);
  node_setup_const(k2, 2.0);

  root->children[0] = sub;
  sub->children[0] = k1;
  sub->children[1] = k2;

  /* Right */
  node_t *mul = node_new();
  node_t *k3 = node_new();
  node_t *k4 = node_new();
  node_setup_func(mul, MUL, 2);
  node_setup_const(k3, 3.0);
  node_setup_const(k4, 4.0);

  root->children[1] = mul;
  mul->children[0] = k3;
  mul->children[1] = k4;

  /* Assert */
  tree_update(&t);
  int idx = tree_select_rand_func(&t);
  /* printf("idx: %d\n", idx); */
  MU_CHECK(idx == 0 || idx == 1 || idx == 4 || idx == 6);

  return 0;
}

int test_subtree_size() {
  /* Setup */
  tree_t t;
  tree_setup(&t);

  /* Root */
  node_t *root = node_new();
  node_setup_func(root, ADD, 2);
  t.root = root;

  /* Left */
  node_t *sub = node_new();
  node_t *k1 = node_new();
  node_t *k2 = node_new();

  node_setup_func(sub, SUB, 2);
  node_setup_const(k1, 1.0);
  node_setup_const(k2, 2.0);

  root->children[0] = sub;
  sub->children[0] = k1;
  sub->children[1] = k2;

  /* Right */
  node_t *mul = node_new();
  node_t *k3 = node_new();
  node_t *pow = node_new();
  node_t *k4 = node_new();
  node_t *k5 = node_new();
  node_setup_func(mul, MUL, 2);
  node_setup_const(k3, 3.0);
  node_setup_func(pow, POW, 2);
  node_setup_const(k4, 4.0);
  node_setup_const(k5, 5.0);

  root->children[1] = mul;
  mul->children[0] = k3;
  mul->children[1] = pow;
  pow->children[0] = k4;
  pow->children[1] = k5;

  /* Assert */
  tree_update(&t);
  tree_print(&t);
  MU_CHECK(subtree_size(root) == t.size);
  MU_CHECK(subtree_size(sub) == 3);
  MU_CHECK(subtree_size(mul) == 5);

  /* Clean up */
  tree_clear(&t);

  return 0;
}

/******************************************************************************
 *                             MUTATION OPERATORS
 ******************************************************************************/

int test_point_mutation() {
  /* Setup function set */
  function_set_t fs;
  fs.funcs = (int[7]) {ADD, SUB, MUL, DIV, POW, EXP, LOG};
  fs.arity = (int[7]) {ADD_ARITY, SUB_ARITY, MUL_ARITY, DIV_ARITY,
                       POW_ARITY, EXP_ARITY, LOG_ARITY};
  fs.length = 7;
  function_set_print(&fs);

  /* Setup terminal set */
  terminal_set_t ts;
  ts.terms = (terminal_t[13]) {
    {.type = CONST, .val = 0.0},
    {.type = CONST, .val = 1.0},
    {.type = CONST, .val = 2.0},
    {.type = CONST, .val = 3.0},
    {.type = CONST, .val = 4.0},
    {.type = CONST, .val = 5.0},
    {.type = CONST, .val = 6.0},
    {.type = CONST, .val = 7.0},
    {.type = CONST, .val = 8.0},
    {.type = CONST, .val = 9.0},
    {.type = INPUT, .str = "x"},
    {.type = INPUT, .str = "y"},
    {.type = RCONST, .range = {-1.0, 1.0}},
  };
  ts.length = 13;
  terminal_set_print(&ts);

  /* Generate tree */
  tree_t t;
  tree_generate(GROW, &fs, &ts, 1, &t);

  printf("BEFORE\n");
  tree_print(&t);

  point_mutation(&fs, &ts, &t);

  printf("\nAFTER\n");
  tree_print(&t);

  /* Clean up */
  tree_clear(&t);

  return 0;
}

/******************************************************************************
 *                             CROSSOVER OPERATORS
 ******************************************************************************/

int test_point_crossover() {
  /* Setup function set */
  function_set_t fs;
  fs.funcs = (int[7]) {ADD, SUB, MUL, DIV, POW, EXP, LOG};
  fs.arity = (int[7]) {ADD_ARITY, SUB_ARITY, MUL_ARITY, DIV_ARITY,
                       POW_ARITY, EXP_ARITY, LOG_ARITY};
  fs.length = 7;
  function_set_print(&fs);

  /* Setup terminal set */
  terminal_set_t ts;
  ts.terms = (terminal_t[13]) {
    {.type = CONST, .val = 0.0},
    {.type = CONST, .val = 1.0},
    {.type = CONST, .val = 2.0},
    {.type = CONST, .val = 3.0},
    {.type = CONST, .val = 4.0},
    {.type = CONST, .val = 5.0},
    {.type = CONST, .val = 6.0},
    {.type = CONST, .val = 7.0},
    {.type = CONST, .val = 8.0},
    {.type = CONST, .val = 9.0},
    {.type = INPUT, .str = "x"},
    {.type = INPUT, .str = "y"},
    {.type = RCONST, .range = {-1.0, 1.0}},
  };
  ts.length = 13;
  terminal_set_print(&ts);

  /* Generate tree */
  tree_t t1;
  tree_t t2;
  tree_generate(FULL, &fs, &ts, 2, &t1);
  tree_generate(FULL, &fs, &ts, 2, &t2);
  printf("tree1:\n"); tree_print(&t1); printf("\n");
  printf("tree2:\n"); tree_print(&t2);

  printf("\n");
  point_crossover(&t1, &t2);

  printf("tree1:\n"); tree_print(&t1); printf("\n");
  printf("tree2:\n"); tree_print(&t2);

  /* clean up */
  tree_clear(&t1);
  tree_clear(&t2);

  return 0;
}

void test_suite() {
  srand(time(NULL));

  /* NODE */
  MU_ADD_TEST(test_node_setup);
  MU_ADD_TEST(test_node_clear);
  MU_ADD_TEST(test_node_setup_func);
  MU_ADD_TEST(test_node_setup_input);
  MU_ADD_TEST(test_node_setup_const);
  MU_ADD_TEST(test_node_new_and_delete);
  MU_ADD_TEST(test_random_func);
  MU_ADD_TEST(test_random_term);

  /* TREE */
  MU_ADD_TEST(test_tree_setup);
  MU_ADD_TEST(test_tree_clear);
  MU_ADD_TEST(test_tree_generate);
  MU_ADD_TEST(test_tree_update);
  MU_ADD_TEST(test_tree_get_node);
  MU_ADD_TEST(test_tree_select_rand_func);
  MU_ADD_TEST(test_subtree_size);

  /* MUTATION OPERATORS */
  MU_ADD_TEST(test_point_mutation);

  /* CROSSOVER OPERATORS */
  MU_ADD_TEST(test_point_crossover);
}

MU_RUN_TESTS(test_suite);
