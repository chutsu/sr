#include <string.h>

#include "sr/munit.h"
#include "sr/sr.h"

#define CSV_TEST_DATA "./sr/tests/test_data.csv"

/******************************************************************************
 *                                  STACK
 ******************************************************************************/

int test_stack_setup() {
  stack_t s;
  stack_setup(&s);

  for (int i = 0; i < MAX_TREE_SIZE; i++) {
    MU_CHECK(s.data[i] == NULL);
  }
  MU_CHECK(s.size == 0);

  return 0;
}

int test_stack_push() {
  stack_t s;
  stack_setup(&s);

  int data[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  for (int i = 0; i < 10; i++) {
    stack_push(&s, data + i);
  }

  for (int i = 0; i < 10; i++) {
    MU_CHECK(*(int *) s.data[i] == (i + 1));
  }
  MU_CHECK(s.size == 10);

  return 0;
}

int test_stack_pop() {
  stack_t s;
  stack_setup(&s);

  int data[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  for (int i = 0; i < 10; i++) {
    stack_push(&s, data + i);
  }

  for (int i = 0; i < 10; i++) {
    int item = *(int *) stack_pop(&s);
    printf("item: %d\n", item);
    MU_CHECK(item == (10 - i));
  }
  MU_CHECK(s.size == 0);

  return 0;
}

/******************************************************************************
 *                                   NODE
 ******************************************************************************/

int test_node_new_and_delete() {
  node_t *n = node_new();

  /* General */
  MU_CHECK(n->type == -1);
  MU_CHECK(n->parent == NULL);
  MU_CHECK(n->nth_child == -1);

  /* Terminal specific */
  MU_CHECK(fltcmp(n->value, 0.0) == 0);

  /* Function specific */
  MU_CHECK(n->function == -1);
  MU_CHECK(n->arity == -1);
  for (int i = 0; i < MAX_ARITY; i++) {
    MU_CHECK(n->children[i] == NULL);
  }

	node_delete(n);

  return 0;
}

int test_node_copy() {
  node_t *mul= node_new_func(MUL, MUL_ARITY);
  node_t *add = node_new_func(ADD, ADD_ARITY);
  node_t *sub = node_new_func(SUB, SUB_ARITY);
  node_t *x = node_new_input("x");
  node_t *y = node_new_input("y");
  node_t *k1 = node_new_const(1.0);
  node_t *k2 = node_new_const(2.0);

  mul->children[0] = add;
  mul->children[1] = sub;
  add->children[0] = x;
  add->children[1] = y;
  sub->children[0] = k1;
  sub->children[1] = k2;

  tree_t *t1 = tree_new();
  t1->root = mul;
  tree_update(t1);
  tree_print(t1);

  node_t *src = mul;
  node_t *des = node_copy(src);

  node_delete(src);
  free(t1);

  tree_t *t2 = tree_new();
  t2->root = des;
  tree_update(t2);
  tree_print(t2);
  node_delete(des);
  free(t2);

  return 0;
}

int test_node_string() {
  node_t n;
  char *s;

  /* TERM NODE */
  /* -- INPUT NODE */
  n.type = TERM_NODE;
  n.data_type = INPUT;
  n.input_name = malloc_string("x");
  s = node_string(&n);
  MU_CHECK(strcmp(s, "x") == 0);
  free(n.input_name);
  free(s);
  /* -- CONST NODE */
  n.type = TERM_NODE;
  n.data_type = CONST;
  n.value = -1.0000;
  s = node_string(&n);
  MU_CHECK(strcmp(s, "-1.0000e+00") == 0);
  free(s);

  /* FUNC NODE */
  /* -- ADD */
  n.type = FUNC_NODE;
  n.function = ADD;
  s = node_string(&n);
  MU_CHECK(strcmp(s, "ADD") == 0);
  free(s);
  /* -- SUB */
  n.type = FUNC_NODE;
  n.function = SUB;
  s = node_string(&n);
  MU_CHECK(strcmp(s, "SUB") == 0);
  free(s);
  /* -- MUL */
  n.type = FUNC_NODE;
  n.function = MUL;
  s = node_string(&n);
  MU_CHECK(strcmp(s, "MUL") == 0);
  free(s);
  /* -- DIV */
  n.type = FUNC_NODE;
  n.function = DIV;
  s = node_string(&n);
  MU_CHECK(strcmp(s, "DIV") == 0);
  free(s);
  /* -- POW */
  n.type = FUNC_NODE;
  n.function = POW;
  s = node_string(&n);
  MU_CHECK(strcmp(s, "POW") == 0);
  free(s);
  /* -- EXP */
  n.type = FUNC_NODE;
  n.function = EXP;
  s = node_string(&n);
  MU_CHECK(strcmp(s, "EXP") == 0);
  free(s);
  /* -- LOG */
  n.type = FUNC_NODE;
  n.function = LOG;
  s = node_string(&n);
  MU_CHECK(strcmp(s, "LOG") == 0);
  free(s);
  /* -- SIN */
  n.type = FUNC_NODE;
  n.function = SIN;
  s = node_string(&n);
  MU_CHECK(strcmp(s, "SIN") == 0);
  free(s);
  /* -- COS */
  n.type = FUNC_NODE;
  n.function = COS;
  s = node_string(&n);
  MU_CHECK(strcmp(s, "COS") == 0);
  free(s);

  return 0;
}

int test_node_new_func() {
  node_t *n = node_new_func(ADD, ADD_ARITY);

  MU_CHECK(n->type == FUNC_NODE);
  MU_CHECK(n->function == ADD);
  MU_CHECK(n->arity == ADD_ARITY);
	node_delete(n);

  return 0;
}

int test_node_new_input() {
  node_t *n = node_new_input("x");

  MU_CHECK(n->type == TERM_NODE);
  MU_CHECK(n->data_type == INPUT);
  MU_CHECK(strcmp(n->input_name, "x") == 0);
	node_delete(n);

  return 0;
}

int test_node_new_const() {
  node_t *n = node_new_const(1.0);

  MU_CHECK(n->type == TERM_NODE);
  MU_CHECK(n->data_type == CONST);
  MU_CHECK(fltcmp(n->value, 1.0) == 0);
	node_delete(n);

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
  node_t *n = random_func(&fs);

  /* Assert */
  node_print(n);
  MU_CHECK(n->type == FUNC_NODE);
  MU_CHECK(n->function >= 0 || n->function <= 6);
  MU_CHECK(n->arity >= 0 || n->arity <= 2);
	node_delete(n);

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
  node_t *n = random_term(&ts);

  /* Assert */
  node_print(n);
  MU_CHECK(n->type == TERM_NODE);
	node_delete(n);

  return 0;
}

/******************************************************************************
 *                                   TREE
 ******************************************************************************/

int test_tree_new_and_delete() {
  tree_t *t = tree_new();

  MU_CHECK(t->root == NULL);
  MU_CHECK(t->size == 0);
  MU_CHECK(t->depth == 0);

  MU_CHECK(fltcmp(t->error, 0.0) == 0);
  MU_CHECK(fltcmp(t->score, 0.0) == 0);

  tree_delete(t);

  return 0;
}

int test_tree_copy() {
  /* Setup function set */
  function_set_t fs;
  fs.funcs = (int[7]) {ADD, SUB, MUL, DIV, POW, EXP, LOG};
  fs.arity = (int[7]) {ADD_ARITY, SUB_ARITY, MUL_ARITY, DIV_ARITY,
                       POW_ARITY, EXP_ARITY, LOG_ARITY};
  fs.length = 7;
  /* function_set_print(&fs); */

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
  /* terminal_set_print(&ts); */

  /* Generate tree */
  tree_t *t1 = tree_generate(FULL, &fs, &ts, 2);
  tree_t *t2 = tree_copy(t1);

  tree_print(t1);
  tree_print(t2);

  tree_delete(t1);
  tree_delete(t2);

  return 0;
}

int test_tree_string() {
  /* Setup */
  tree_t *t = tree_new();

  /* Root */
  node_t *root = node_new_func(ADD, 2);
  t->root = root;

  /* Depth 1 - Left */
  node_t *sub = node_new_func(SUB, 2);
  node_t *k1 = node_new_const(1.0);
  node_t *k2 = node_new_const(2.0);

  root->children[0] = sub;
  sub->children[0] = k1;
  sub->children[1] = k2;

  /* Depth 1 - Right */
  node_t *mul = node_new_func(MUL, 2);
  node_t *k3 = node_new_const(3.0);
  node_t *k4 = node_new_const(4.0);

  root->children[1] = mul;
  mul->children[0] = k3;
  mul->children[1] = k4;

  tree_print(t);
  tree_update(t);

  char *s = tree_string(t);
  printf("%s\n", s);
  free(s);

  tree_delete(t);

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
  tree_t *t = tree_generate(FULL, &fs, &ts, 2);
  MU_CHECK(t != NULL);

  /* Print tree */
  tree_print(t);
  /* MU_CHECK(t.nodes[t.size + 1].type == -1); */
	tree_delete(t);

  return 0;
}

int test_tree_update() {
  /* Setup */
  tree_t *t = tree_new();

  /* Root */
  node_t *root = node_new_func(ADD, 2);
  t->root = root;

  /* Depth 1 - Left */
  node_t *sub = node_new_func(SUB, 2);
  node_t *k1 = node_new_const(1.0);
  node_t *k2 = node_new_const(2.0);

  root->children[0] = sub;
  sub->children[0] = k1;
  sub->children[1] = k2;

  /* Depth 1 - Right */
  node_t *mul = node_new_func(MUL, 2);
  node_t *k3 = node_new_const(3.0);
  node_t *k4 = node_new_const(4.0);

  root->children[1] = mul;
  mul->children[0] = k3;
  mul->children[1] = k4;

  tree_print(t);
  tree_update(t);
	tree_delete(t);

  return 0;
}

int test_tree_get_node() {
  /* Setup */
  tree_t *t = tree_new();

  /* Root */
  node_t *root = node_new_func(ADD, 2);
  t->root = root;

  /* Left */
  node_t *sub = node_new_func(SUB, 2);
  node_t *k1 = node_new_const(1.0);
  node_t *k2 = node_new_const(2.0);

  root->children[0] = sub;
  sub->children[0] = k1;
  sub->children[1] = k2;

  /* Right */
  node_t *mul = node_new_func(MUL, 2);
  node_t *k3 = node_new_const(3.0);
  node_t *k4 = node_new_const(4.0);

  root->children[1] = mul;
  mul->children[0] = k3;
  mul->children[1] = k4;

  /* Assert */
  tree_print(t);
  {
    node_t *n = tree_get_node(t, 0);
    MU_CHECK(n->type == FUNC_NODE);
    MU_CHECK(n->function == ADD);
    MU_CHECK(n->arity == 2);
  }

  {
    node_t *n = tree_get_node(t, 1);
    MU_CHECK(n->type == FUNC_NODE);
    MU_CHECK(n->function == SUB);
    MU_CHECK(n->arity == 2);
  }

  {
    node_t *n = tree_get_node(t, 2);
    MU_CHECK(n->type == TERM_NODE);
    MU_CHECK(n->data_type == CONST);
    MU_CHECK(fltcmp(n->value, 1.0) == 0);
  }

  {
    node_t *n = tree_get_node(t, 3);
    MU_CHECK(n->type == TERM_NODE);
    MU_CHECK(n->data_type == CONST);
    MU_CHECK(fltcmp(n->value, 2.0) == 0);
  }

  {
    node_t *n = tree_get_node(t, 4);
    MU_CHECK(n->type == FUNC_NODE);
    MU_CHECK(n->function == MUL);
    MU_CHECK(n->arity == 2);
  }

  {
    node_t *n = tree_get_node(t, 5);
    MU_CHECK(n->type == TERM_NODE);
    MU_CHECK(n->data_type == CONST);
    MU_CHECK(fltcmp(n->value, 3.0) == 0);
  }

  {
    node_t *n = tree_get_node(t, 6);
    MU_CHECK(n->type == TERM_NODE);
    MU_CHECK(n->data_type == CONST);
    MU_CHECK(fltcmp(n->value, 4.0) == 0);
  }

  {
    node_t *n = tree_get_node(t, 9);
    MU_CHECK(n == NULL);
  }

  /* Clean up */
	tree_delete(t);

  return 0;
}

int test_tree_select_rand_func() {
  /* Setup */
  tree_t *t = tree_new();

  /* Root */
  node_t *root = node_new_func(ADD, 2);
  t->root = root;

  /* Left */
  node_t *sub = node_new_func(SUB, 2);
  node_t *k1 = node_new_const(1.0);
  node_t *k2 = node_new_const(2.0);

  root->children[0] = sub;
  sub->children[0] = k1;
  sub->children[1] = k2;

  /* Right */
  node_t *mul = node_new_func(MUL, 2);
  node_t *k3 = node_new_const(3.0);
  node_t *k4 = node_new_const(4.0);

  root->children[1] = mul;
  mul->children[0] = k3;
  mul->children[1] = k4;

  /* Assert */
  tree_update(t);
  int idx = tree_select_rand_func(t);
  /* printf("idx: %d\n", idx); */
  MU_CHECK(idx == 0 || idx == 1 || idx == 4 || idx == 6);
	tree_delete(t);

  return 0;
}

int test_tree_stack() {
  /* Setup */
  tree_t *t = tree_new();

  /* Root */
  node_t *root = node_new_func(ADD, 2);
  t->root = root;

  /* Left */
  node_t *sub = node_new_func(SUB, 2);
  node_t *k1 = node_new_const(1.0);
  node_t *k2 = node_new_const(2.0);

  root->children[0] = sub;
  sub->children[0] = k1;
  sub->children[1] = k2;

  /* Right */
  node_t *mul = node_new_func(MUL, 2);
  node_t *k3 = node_new_const(3.0);
  node_t *pow = node_new_func(POW, 2);
  node_t *k4 = node_new_const(4.0);
  node_t *k5 = node_new_const(5.0);

  root->children[1] = mul;
  mul->children[0] = k3;
  mul->children[1] = pow;
  pow->children[0] = k4;
  pow->children[1] = k5;

  /* Assert */
  tree_update(t);
  /* tree_print(&t); */

  stack_t s;
	stack_setup(&s);
  tree_stack(t, &s);
  for (int i = 0; i < s.size; i++) {
    node_print(s.data[i]);
  }

	/* Clean up */
	tree_delete(t);

  return 0;
}

int test_subtree_size() {
  /* Setup */
  tree_t *t = tree_new();

  /* Root */
  node_t *root = node_new_func(ADD, 2);
  t->root = root;

  /* Left */
  node_t *sub = node_new_func(SUB, 2);
  node_t *k1 = node_new_const(1.0);
  node_t *k2 = node_new_const(2.0);

  root->children[0] = sub;
  sub->children[0] = k1;
  sub->children[1] = k2;

  /* Right */
  node_t *mul = node_new_func(MUL, 2);
  node_t *k3 = node_new_const(3.0);
  node_t *pow = node_new_func(POW, 2);
  node_t *k4 = node_new_const(4.0);
  node_t *k5 = node_new_const(5.0);

  root->children[1] = mul;
  mul->children[0] = k3;
  mul->children[1] = pow;
  pow->children[0] = k4;
  pow->children[1] = k5;

  /* Assert */
  tree_update(t);
  tree_print(t);
  MU_CHECK(subtree_size(root) == t->size);
  MU_CHECK(subtree_size(sub) == 3);
  MU_CHECK(subtree_size(mul) == 5);

  /* Clean up */
	tree_delete(t);

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
  tree_t *t = tree_generate(GROW, &fs, &ts, 2);

  printf("BEFORE\n");
  tree_print(t);

  point_mutation(&fs, &ts, t);

  printf("\nAFTER\n");
  tree_print(t);

  /* Clean up */
  tree_delete(t);

  return 0;
}

int test_subtree_mutation() {
  /* Setup function set */
  function_set_t fs;
  fs.funcs = (int[7]) {ADD, SUB, MUL, DIV, POW, EXP, LOG};
  fs.arity = (int[7]) {ADD_ARITY, SUB_ARITY, MUL_ARITY, DIV_ARITY,
                       POW_ARITY, EXP_ARITY, LOG_ARITY};
  fs.length = 7;

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

  /* Test subtree mutation */
  for (int i = 0; i < 100; i++) {
    /* Generate tree */
    tree_t *t = tree_generate(GROW, &fs, &ts, 2);

    char *t_before = tree_string(t);
    printf("BEFORE: %s\n", t_before);

    subtree_mutation(&fs, &ts, t);

    char *t_after = tree_string(t);
    printf("AFTER:  %s\n", t_after);

    MU_CHECK(strcmp(t_before, t_after) != 0);

    /* Clean up */
    free(t_before);
    free(t_after);
    tree_delete(t);
  }

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
  /* function_set_print(&fs); */

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
  /* terminal_set_print(&ts); */

  /* Generate tree */
  tree_t *t1 = tree_generate(FULL, &fs, &ts, 3);
  tree_t *t2 = tree_generate(FULL, &fs, &ts, 3);
  char *t1_str = NULL;
  char *t2_str = NULL;

  /* Before */
  t1_str = tree_string(t1);
  t2_str = tree_string(t2);
  printf("tree1: %s\n", t1_str);
  printf("tree2: %s\n", t2_str);
  printf("\n");
  free(t1_str);
  free(t2_str);

  /* Point crossover */
  point_crossover(t1, t2);

  /* After */
  t1_str = tree_string(t1);
  t2_str = tree_string(t2);
  printf("tree1: %s\n", t1_str);
  printf("tree2: %s\n", t2_str);
  printf("\n");
  free(t1_str);
  free(t2_str);

  /* clean up */
  tree_delete(t1);
  tree_delete(t2);

  return 0;
}

/******************************************************************************
 *                          SELECTION OPERATORS
 ******************************************************************************/

int test_tournament_selection() {
  /* Setup function set */
  function_set_t fs;
  fs.funcs = (int[7]) {ADD, SUB, MUL, DIV, POW, EXP, LOG};
  fs.arity = (int[7]) {ADD_ARITY, SUB_ARITY, MUL_ARITY, DIV_ARITY,
                       POW_ARITY, EXP_ARITY, LOG_ARITY};
  fs.length = 7;

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

  /* Generate trees */
  tree_t **trees = malloc(sizeof(tree_t *) * 10);
  for (int i = 0; i < 10; i++) {
    trees[i] = tree_generate(FULL, &fs, &ts, 2);
    trees[i]->score = i;
    printf("score: %f\n", trees[i]->score);
  }
  printf("---\n");

  /* Perform selection */
  trees = tournament_selection(trees, 10, 10000);
  for (int i = 0; i < 10; i++) {
    printf("score: %f\n", trees[i]->score);
    MU_CHECK(fltcmp(trees[i]->score, 0.0) == 0);
  }

  /* Delete trees */
  for (int i = 0; i < 10; i++) {
    tree_delete(trees[i]);
  }
  free(trees);

  return 0;
}

/******************************************************************************
 *                              REGRESSION
 ******************************************************************************/

int test_csv_rows() {
	const int nb_rows = csv_rows(CSV_TEST_DATA);

	/* printf("nb_rows: %d\n", nb_rows); */
	MU_CHECK(nb_rows == 101);

	return 0;
}

int test_csv_cols() {
	const int nb_cols = csv_cols(CSV_TEST_DATA);

	/* printf("nb_cols: %d\n", nb_cols); */
	MU_CHECK(nb_cols == 2);

	return 0;
}

int test_csv_fields() {
	int nb_fields = 0;
	char **fields = csv_fields(CSV_TEST_DATA, &nb_fields);

	/* for (int i = 0; i < nb_fields; i++) { */
	/* 	printf("field[%d]: [%s]\n", i, fields[i]); */
	/* } */
	MU_CHECK(nb_fields == 2);
	MU_CHECK(strcmp(fields[0], "x") == 0);
	MU_CHECK(strcmp(fields[1], "y") == 0);

	for (int i = 0; i < nb_fields; i++) {
		free(fields[i]);
	}
	free(fields);

	return 0;
}

int test_csv_data() {
	int nb_rows = 0;
	int nb_cols = 0;
	double **data = csv_data(CSV_TEST_DATA, &nb_rows, &nb_cols);

	/* for (int i = 0; i < nb_rows; i++) { */
	/* 	printf("%f, %f\n", data[0][i], data[1][i]); */
	/* } */
	MU_CHECK(nb_rows == 101);
	MU_CHECK(nb_cols == 2);

	for (int i = 0; i < nb_cols; i++) {
		free(data[i]);
	}
	free(data);

	return 0;
}

int test_dataset_load_and_delete() {
	dataset_t *ds = dataset_load(CSV_TEST_DATA, "x");

	/* printf("%s, %s\n", ds->fields[0], ds->fields[1]); */
	/* for (int i = 0; i < ds->nb_rows; i++) { */
	/* 	printf("%f, %f\n", ds->data[0][i], ds->data[1][i]); */
	/* } */

	MU_CHECK(ds->nb_rows == 101);
	MU_CHECK(ds->nb_cols == 2);
	MU_CHECK(strcmp(ds->fields[0], "x") == 0);
	MU_CHECK(strcmp(ds->fields[1], "y") == 0);
	MU_CHECK(strcmp(ds->predict, "x") == 0);

	dataset_delete(ds);

  return 0;
}

int test_evaluate_tree() {
  /* Setup function set */
  function_set_t fs;
  fs.funcs = (int[7]) {ADD, SUB, MUL, DIV, POW, EXP, LOG};
  fs.arity = (int[7]) {ADD_ARITY, SUB_ARITY, MUL_ARITY, DIV_ARITY,
                       POW_ARITY, EXP_ARITY, LOG_ARITY};
  fs.length = 7;
  function_set_print(&fs);

  /* Setup terminal set */
  terminal_set_t ts;
  ts.terms = (terminal_t[12]) {
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
    {.type = RCONST, .range = {-1.0, 1.0}},
  };
  ts.length = 12;
  terminal_set_print(&ts);

  /* Load dataset and generate a tree */
	dataset_t *ds = dataset_load(CSV_TEST_DATA, "y");
  tree_t *t = tree_generate(FULL, &fs, &ts, 2);
  evaluate_tree(t, ds);
  tree_print(t);

	/* Clean up */
	tree_delete(t);
	dataset_delete(ds);

  return 0;
}

int test_best_tree() {
  /* Setup function set */
  function_set_t fs;
  fs.funcs = (int[7]) {ADD, SUB, MUL, DIV, POW, EXP, LOG};
  fs.arity = (int[7]) {ADD_ARITY, SUB_ARITY, MUL_ARITY, DIV_ARITY,
                       POW_ARITY, EXP_ARITY, LOG_ARITY};
  fs.length = 7;
  function_set_print(&fs);

  /* Setup terminal set */
  terminal_set_t ts;
  ts.terms = (terminal_t[12]) {
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
    {.type = RCONST, .range = {-1.0, 1.0}},
  };
  ts.length = 12;
  terminal_set_print(&ts);

  /* Setup trees */
  tree_t **trees = malloc(sizeof(tree_t) * 10);
  for (int i = 0; i < 10; i++) {
    trees[i] = tree_generate(FULL, &fs, &ts, 2);
    trees[i]->score = i;
  }

  /* Find best */
  tree_t *best = best_tree(trees, 10);
  MU_CHECK(fltcmp(best->score, 0.0) == 0);

  /* Clean up */
  for (int i = 0; i < 10; i++) {
    tree_delete(trees[i]);
  }
  free(trees);

  return 0;
}

int test_regress() {
  /* Setup function set */
  function_set_t fs;
  fs.funcs = (int[4]) {ADD, SUB, MUL, DIV};
  fs.arity = (int[4]) {ADD_ARITY, SUB_ARITY, MUL_ARITY,
                       DIV_ARITY};
  fs.length = 4;

  /* Setup terminal set */
  terminal_set_t ts;
  ts.terms = (terminal_t[12]) {
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
    {.type = RCONST, .range = {-1.0, 1.0}},
  };
  ts.length = 12;

  /* Load dataset and generate trees */
	dataset_t *ds = dataset_load(CSV_TEST_DATA, "y");
	int nb_trees = 1000;
	tree_t **trees = malloc(sizeof(tree_t *) * nb_trees);
	for (int i = 0; i < nb_trees; i++) {
    trees[i] = tree_generate(FULL, &fs, &ts, 3);
	}

  int max_iter = 1000;
  int iter = 0;
	while (iter != max_iter) {
	  /* Crossover */
	  for (int i = 0; i < nb_trees; i+=2) {
	    if (randf(0.0, 1.0) < 0.8) {
        point_crossover(trees[i], trees[i + 1]);
      }
    }

    /* Mutate */
	  for (int i = 0; i < nb_trees; i++) {
	    if (randf(0.0, 1.0) < 0.1) {
        /* point_mutation(&fs, &ts, trees[i]); */
        subtree_mutation(&fs, &ts, trees[i]);
      }
    }

    /* Evaluate */
	  for (int i = 0; i < nb_trees; i++) {
      evaluate_tree(trees[i], ds);
	  }

    /* Selection */
    trees = tournament_selection(trees, nb_trees, nb_trees * 0.1);

	  /* Show the best */
	  tree_t *best = best_tree(trees, nb_trees);
	  printf("iter[%d] score: %f\t error:%f\n", iter, best->score, best->error);

	  /* for (int i = 0; i < nb_trees; i++) { */
	  /*   const tree_t *t = trees[i]; */
	  /*   char *t_str = tree_string(t); */
    /*   printf("score: %f\t error:%f \t %s\n", t->score, t->error, t_str); */
    /*   free(t_str); */
	  /* } */
	  /* printf("----------\n"); */

	  iter++;
	}

	/* Clean up */
	for (int i = 0; i < nb_trees; i++) {
    tree_delete(trees[i]);
  }
  free(trees);
	dataset_delete(ds);

  return 0;
}

/******************************************************************************
 *                              TEST SUITE
 ******************************************************************************/

void test_suite() {
  srand(time(NULL));

  /* STACK */
  MU_ADD_TEST(test_stack_setup);
  MU_ADD_TEST(test_stack_push);
  MU_ADD_TEST(test_stack_pop);

  /* NODE */
  MU_ADD_TEST(test_node_new_and_delete);
  MU_ADD_TEST(test_node_copy);
  MU_ADD_TEST(test_node_string);
  MU_ADD_TEST(test_node_new_func);
  MU_ADD_TEST(test_node_new_input);
  MU_ADD_TEST(test_node_new_const);
  MU_ADD_TEST(test_random_func);
  MU_ADD_TEST(test_random_term);

  /* TREE */
  MU_ADD_TEST(test_tree_new_and_delete);
  MU_ADD_TEST(test_tree_copy);
  MU_ADD_TEST(test_tree_string);
  MU_ADD_TEST(test_tree_generate);
  MU_ADD_TEST(test_tree_update);
  MU_ADD_TEST(test_tree_get_node);
  MU_ADD_TEST(test_tree_select_rand_func);
  MU_ADD_TEST(test_tree_stack);
  MU_ADD_TEST(test_subtree_size);

  /* MUTATION OPERATORS */
  MU_ADD_TEST(test_point_mutation);
  MU_ADD_TEST(test_subtree_mutation);

  /* CROSSOVER OPERATORS */
  MU_ADD_TEST(test_point_crossover);

  /* SELECTION OPERATORS */
  MU_ADD_TEST(test_tournament_selection);

  /* REGRESSION */
  MU_ADD_TEST(test_csv_rows);
  MU_ADD_TEST(test_csv_cols);
  MU_ADD_TEST(test_csv_fields);
  MU_ADD_TEST(test_csv_data);
  MU_ADD_TEST(test_dataset_load_and_delete);
  MU_ADD_TEST(test_evaluate_tree);
  MU_ADD_TEST(test_best_tree);
  MU_ADD_TEST(test_regress);
}

MU_RUN_TESTS(test_suite);
