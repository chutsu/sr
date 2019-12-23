#ifndef SR_H
#define SR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "sr/utils.h"

/* PARAMETERS */
#define MAX_ARITY 10
#define MAX_TREE_SIZE 500

/******************************************************************************
 *                                  STACK
 ******************************************************************************/
struct stack_t {
  void *data[MAX_TREE_SIZE];
  size_t size;
} typedef stack_t;

void stack_setup(stack_t *s) {
  for (int i = 0; i < MAX_TREE_SIZE; i++) {
    s->data[i] = NULL;
  }
  s->size = 0.0;
}

void stack_push(stack_t *s, void *item) {
  s->data[s->size] = item;
  s->size++;
}

void *stack_pop(stack_t *s) {
  s->size--;
  return s->data[s->size];
}

/******************************************************************************
 *                                FUNCTION SET
 ******************************************************************************/

/* Function Types */
#define ADD 0
#define SUB 1
#define MUL 2
#define DIV 3
#define POW 4
#define EXP 5
#define LOG 6
#define SIN 7
#define COS 8

/* Function Arity */
#define ADD_ARITY 2
#define SUB_ARITY 2
#define MUL_ARITY 2
#define DIV_ARITY 2
#define POW_ARITY 2
#define EXP_ARITY 1
#define LOG_ARITY 1
#define SIN_ARITY 1
#define COS_ARITY 1

typedef struct function_set_t {
  int *funcs;
  int *arity;
  int length;
} function_set_t;

void function_set_print(const function_set_t *fs) {
  assert(fs != NULL);

  printf("Function set:\n");
  printf("------------------------------\n");
  for (int i = 0; i < fs->length; i++) {
    switch (fs->funcs[i]) {
    case ADD: printf("[%d]: ADD\tarity: %d\n", i, fs->arity[i]); break;
    case SUB: printf("[%d]: SUB\tarity: %d\n", i, fs->arity[i]); break;
    case MUL: printf("[%d]: MUL\tarity: %d\n", i, fs->arity[i]); break;
    case DIV: printf("[%d]: DIV\tarity: %d\n", i, fs->arity[i]); break;
    case POW: printf("[%d]: POW\tarity: %d\n", i, fs->arity[i]); break;
    case EXP: printf("[%d]: EXP\tarity: %d\n", i, fs->arity[i]); break;
    case LOG: printf("[%d]: LOG\tarity: %d\n", i, fs->arity[i]); break;
    case SIN: printf("[%d]: SIN\tarity: %d\n", i, fs->arity[i]); break;
    case COS: printf("[%d]: COS\tarity: %d\n", i, fs->arity[i]); break;
    }
  }
  printf("\n");
}

/******************************************************************************
 *                                TERMINAL SET
 ******************************************************************************/

/* Terminal Types */
#define INPUT 0
#define CONST 1
#define RCONST 2
#define EVAL 3

typedef struct terminal_t {
  int type;
  union {
    char *str;
    double val;
    double range[2];
  };
} terminal_t;

typedef struct terminal_set_t {
  terminal_t *terms;
  int length;
} terminal_set_t;

void terminal_set_print(const terminal_set_t *ts) {
  assert(ts != NULL);

  printf("Terminal set:\n");
  printf("------------------------------\n");
  for (int i = 0; i < ts->length; i++) {
    printf("[%d]:", i);
    switch (ts->terms[i].type) {
    case INPUT:
      printf("INPUT\t");
      printf("input_name: %s\n", ts->terms[i].str);
      break;
    case CONST:
      printf("CONST\t");
      printf("value: %f\n", ts->terms[i].val);
      break;
    case RCONST:
      printf("RCONST\t");
      const double min = ts->terms[i].range[0];
      const double max = ts->terms[i].range[1];
      printf("range: [%f, %f]\n", min, max);
      break;
    }
  }
  printf("\n");
}

/******************************************************************************
 *                                   NODE
 ******************************************************************************/

/* Node Types */
#define FUNC_NODE 0
#define TERM_NODE 1

/* Data Types */
#define IMPUT 0
#define CONST 1
#define RCONST 2

typedef struct node_t {
  /* General */
  int type;
  struct node_t *parent;
  int nth_child;

  /* Terminal node specific */
  int data_type;
  double value;
  char *input_name;
	double *eval_data;

  /* Function node specific */
  int function;
  int arity;
  struct node_t *children[MAX_ARITY];
} node_t;

node_t *node_new() {
  node_t *n = malloc(sizeof(node_t));

  /* General */
  n->type = -1;
  n->parent = NULL;
  n->nth_child = -1;

  /* Terminal node specific */
  n->data_type = -1;
  n->value = 0.0;
  n->input_name = NULL;
  n->eval_data = NULL;

  /* Function node specific */
  n->function = -1;
  n->arity = -1;
  for (int i = 0; i < MAX_ARITY; i++) {
    n->children[i] = NULL;
  }

	return n;
}

static void node_delete_traverse(node_t *n) {
	if (n == NULL) {
		return;
	}

  if (n->type == TERM_NODE) {
		if (n->input_name != NULL) {
			free(n->input_name);
		}
		if (n->eval_data != NULL) {
			free(n->eval_data);
		}
    free(n);
    n = NULL;
    return;
  }

  for (int i = 0; i < n->arity; i++) {
		node_delete_traverse(n->children[i]);
  }
  free(n);
  n = NULL;
}

void node_delete(node_t *n) {
  node_delete_traverse(n);
  n = NULL;
}

static void node_copy_traverse(const node_t *src, node_t *des) {
  /* General */
  des->type = src->type;
  des->parent = src->parent;
  des->nth_child = src->nth_child;

  /* Terminal node specific */
  des->data_type = src->data_type;
  des->value = src->value;
  if (src->input_name) {
    des->input_name = malloc(sizeof(char) * strlen(src->input_name) + 1);
    strcpy(des->input_name, src->input_name);
  }
  if (src->eval_data) {
    des->eval_data = src->eval_data;
  }

  /* Function node specific */
  des->function = src->function;
  des->arity = src->arity;
  for (int i = 0; i < des->arity; i++) {
    des->children[i] = node_new();
    node_copy_traverse(src->children[i], des->children[i]);
  }
}

node_t *node_copy(const node_t *src) {
  node_t *des = node_new();
  node_copy_traverse(src, des);
  return des;
}

void node_print(const node_t *n) {
  assert(n != NULL);

  printf("type: ");
  switch (n->type) {
  case FUNC_NODE:
    printf("func\t");
    printf("func: ");
    switch (n->function) {
    case ADD: printf("ADD\t"); break;
    case SUB: printf("SUB\t"); break;
    case MUL: printf("MUL\t"); break;
    case DIV: printf("DIV\t"); break;
    case POW: printf("POW\t"); break;
    case EXP: printf("EXP\t"); break;
    case LOG: printf("LOG\t"); break;
    case SIN: printf("SIN\t"); break;
    case COS: printf("COS\t"); break;
    default:
      printf("Opps! Invalid function type[%d]!", n->function);
      break;
    }
    printf("arity: %d\n", n->arity);
    break;
  case TERM_NODE:
    switch (n->data_type) {
    case INPUT:
      printf("input\t");
      printf("input_name: %s\n", n->input_name);
      break;
    case CONST:
      printf("const\t");
      printf("value: %f\n", n->value);
      break;
    }
    break;
  default:
    printf("Opps! Invalid node type [%d]!\n", n->type);
    break;
  }
}

node_t *node_new_func(const int function, const int arity) {
  node_t *n = node_new();
  n->type = FUNC_NODE;
  n->function = function;
  n->arity = arity;
	return n;
}

node_t *node_new_input(char *input_name) {
  node_t *n = node_new();
  n->type = TERM_NODE;
  n->data_type = INPUT;
  n->input_name = malloc(sizeof(char) * strlen(input_name) + 1);
  strcpy(n->input_name, input_name);
	return n;
}

node_t *node_new_const(double value) {
  node_t *n = node_new();
  n->type = TERM_NODE;
  n->data_type = CONST;
  n->value = value;
	return n;
}

node_t *node_new_eval(size_t size) {
  node_t *n = node_new();
  n->type = TERM_NODE;
  n->data_type = EVAL;
  n->eval_data = malloc(sizeof(double) * size);
	return n;
}

node_t *random_func(const function_set_t *fs) {
  assert(fs != NULL);
  assert(fs->length > 1);
  const int idx = randi(0, fs->length);
  return node_new_func(fs->funcs[idx], fs->arity[idx]);
}

node_t *random_term(const terminal_set_t *ts) {
  assert(ts != NULL);
  assert(ts->length > 1);
  const int idx = randi(0, ts->length);
  const terminal_t *term = &ts->terms[idx];

  switch (term->type) {
  case INPUT:
    return node_new_input(term->str);
    break;
  case CONST:
    return node_new_const(term->val);
    break;
  case RCONST:
    return node_new_const(randf(term->range[0], term->range[1]));
    break;
  }

	return NULL;
}

/******************************************************************************
 *                                   TREE
 ******************************************************************************/

typedef struct tree_t {
  node_t *root;
  int size;
  int depth;

  double error;
  double score;
} tree_t;

tree_t *tree_new() {
  tree_t *t = malloc(sizeof(tree_t));
  t->root = NULL;
  t->size = 0;
  t->depth = 0;

  t->error = 0.0;
  t->score = 0.0;
  return t;
}

void tree_delete(tree_t *t) {
  if (t->root != NULL) {
    node_delete(t->root);
  }
  free(t);
}

tree_t *tree_copy(const tree_t *src) {
  tree_t *t = tree_new();

  t->root = node_copy(src->root);
  t->size = src->size;
  t->depth = src->depth;

  t->error = src->error;
  t->score = src->score;

  return t;
}

static void tree_print_traverse(const node_t *n) {
  if (n->type == TERM_NODE) {
    printf("  ");
    node_print(n);
    return;
  }

  printf("  ");
  node_print(n);
  for (int i = 0; i < n->arity; i++) {
    tree_print_traverse(n->children[i]);
  }
}

void tree_print(const tree_t *t) {
  printf("tree.depth: %d\n", t->depth);
  printf("tree.size: %d\n", t->size);
  printf("tree.error: %f\n", t->error);
  printf("tree.score: %f\n", t->score);
  printf("tree.nodes:\n");
  tree_print_traverse(t->root);
}

/* Tree Build Methods */
#define FULL 0
#define GROW 1
#define RAMPED_HALF_AND_HALF 2

static void tree_build(const int method,
                       tree_t *t,
                       node_t *n,
                       const function_set_t *fs,
                       const terminal_set_t *ts,
                       const int curr_depth,
                       const int max_depth) {
  assert(t != NULL);
  assert(n != NULL);
  assert(fs != NULL);
  assert(ts != NULL);

  /* Pre-check */
  if (curr_depth > t->depth) {
    t->depth++;
  }

  /* Build - Depth first traversal manner*/
  for (int i = 0; i < n->arity; i++) {
    if (curr_depth == max_depth) {
      /* Create terminal node */
      node_t *child = random_term(ts);
      child->parent = n;
      child->nth_child = i;
      n->children[i] = child;
      t->size++;

    } else if (method == GROW && randf(0, 1.0) > 0.5) {
      /* Create terminal node */
      node_t *child = random_term(ts);
      child->parent = n;
      child->nth_child = i;
      n->children[i] = child;
      t->size++;

    } else {
      /* Create function node */
      node_t *child = random_func(fs);
      child->parent = n;
      child->nth_child = i;
      n->children[i] = child;
      t->size++;

      /* Recurse deeper */
      tree_build(method, t, child, fs, ts, curr_depth + 1, max_depth);
    }
  }
}

tree_t *tree_generate(const int method,
                      const function_set_t *fs,
                      const terminal_set_t *ts,
                      const int max_depth) {
  assert(method > -1);
  assert(fs != NULL);
  assert(ts != NULL);
  assert(max_depth > 0);

  /* Generate tree */
	tree_t *t = tree_new();
  t->root = random_func(fs);
  t->size++;
  t->depth++;

  switch (method) {
    case FULL:
    case GROW:
      tree_build(method, t, t->root, fs, ts, 1, max_depth);
      break;
    case RAMPED_HALF_AND_HALF:
      if (randf(0.0, 1.0) > 0.5) {
        tree_build(FULL, t, t->root, fs, ts, 1, max_depth);
      } else {
        tree_build(GROW, t, t->root, fs, ts, 1, max_depth);
      }
      break;
    default:
			FATAL("Opps! Unspported tree generation method [%d]!", method);
      return NULL;
  }

  return t;
}

static void tree_update_traverse(tree_t *t, node_t *n, const int depth) {
  assert(t != NULL && n != NULL);

  t->depth = MAX(depth, t->depth);
  if (n->type == FUNC_NODE) {
    for (int i = 0; i < n->arity; i++) {
      n->children[i]->parent = n;
      n->children[i]->nth_child = i;
      tree_update_traverse(t, n->children[i], depth + 1);
    }
  }

  t->size++;
}

void tree_update(tree_t *t) {
  assert(t != NULL);

  /* Reset size and depth */
  t->depth = 0;
  t->size = 0;

  /* Update tree */
  tree_update_traverse(t, t->root, 0);
}

static node_t *tree_get_node_traverse(node_t *n,
                                      const int target_idx,
                                      int *curr_idx) {
  if (target_idx == *curr_idx) {
    return n;
  }

  for (int i = 0; i < n->arity; i++) {
    *curr_idx += 1;
    node_t *child = n->children[i];
    node_t *retval = tree_get_node_traverse(child, target_idx, curr_idx);
    if (retval != NULL) {
      return retval;
    }
  }

  return NULL;
}

node_t *tree_get_node(const tree_t *t, const int idx) {
  int curr_idx = 0;
  return tree_get_node_traverse(t->root, idx, &curr_idx);
}

int tree_select_rand_func(const tree_t *t) {
  assert(t != NULL);

  int attempts = 0;
  int idx = 0;
try_again:
  idx = randi(0, t->size - 1);
  node_t *n = tree_get_node(t, idx);
  if (n->type != FUNC_NODE) {
    attempts++;
    goto try_again;
  } else if (attempts >= 100) {
    FATAL("Failed to get a function!");
  }

  return idx;
}

static void tree_stack_traverse(node_t *n, stack_t *s) {
  if (n->type == TERM_NODE) {
    stack_push(s, n);
    return;
  }

  stack_push(s, n);
  for (int i = 0; i < n->arity; i++) {
    tree_stack_traverse(n->children[i], s);
  }
}

void tree_stack(const tree_t *t, stack_t *s) {
  tree_stack_traverse(t->root, s);
}

static int subtree_size_traverse(const node_t *n, int size) {
  assert(n != NULL);

  if (n->type == FUNC_NODE) {
    for (int i = 0; i < n->arity; i++) {
      size = subtree_size_traverse(n->children[i], size + 1);
    }
  }

  return size;
}

int subtree_size(const node_t *root) {
  int size = 1;
  return subtree_size_traverse(root, size);
}

/******************************************************************************
 *                           MUTATION OPERATORS
 ******************************************************************************/

static void mutate_term_node(const terminal_set_t *ts, node_t *n) {
	/* Clear terminal node */
  n->data_type = -1;
  n->value = 0.0;
	if (n->input_name) {
		free(n->input_name);
		n->input_name = NULL;
	}

	/* Mutate terminal node */
  node_t *new_term = random_term(ts);
	if (new_term->data_type == INPUT) {
		n->data_type = INPUT;
		n->input_name = malloc(sizeof(char) * strlen(new_term->input_name) + 1);
		strcpy(n->input_name, new_term->input_name);
	} else if (new_term->data_type == CONST) {
		n->data_type = CONST;
		n->value = new_term->value;
	}
	node_delete(new_term);
}

static void mutate_func_node(const function_set_t *fs, node_t *n) {
  int attempts = 0;

try_again:
  {
    int i = randi(0, fs->length);
    if (fs->arity[i] != n->arity && attempts < 20) {
      attempts++;
      goto try_again;
    } else if (attempts >= 20) {
      FATAL("Failed to get a function with an arity of %d!", n->arity);
    }
    n->function = fs->funcs[i];
  }
}

void point_mutation(const function_set_t *fs,
                    const terminal_set_t *ts,
                    tree_t *t) {
  const int idx = randi(0, t->size);
  node_t *n = tree_get_node(t, idx);

  if (n->type == TERM_NODE) {
    mutate_term_node(ts, n);
  } else if (n->type == FUNC_NODE) {
    mutate_func_node(fs, n);
  }
}

/******************************************************************************
 *                           CROSSOVER OPERATORS
 ******************************************************************************/

void point_crossover(tree_t *t1, tree_t *t2) {
  const int t1_pt = randi(1, t1->size - 1);
  const int t2_pt = randi(1, t2->size - 1);

  node_t *t1_subtree = tree_get_node(t1, t1_pt);
  node_t *t2_subtree = tree_get_node(t2, t2_pt);
  const int t1_nth_child = t1_subtree->nth_child;
  const int t2_nth_child = t2_subtree->nth_child;
  node_t *t1_parent = t1_subtree->parent;
  node_t *t2_parent = t2_subtree->parent;

  t1_parent->children[t1_nth_child] = t2_subtree;
  t2_parent->children[t2_nth_child] = t1_subtree;

  t1_subtree->parent = t2_parent;
  t2_subtree->parent = t1_parent;

	tree_update(t1);
	tree_update(t2);
}

/******************************************************************************
 *                          SELECTION OPERATORS
 ******************************************************************************/

tree_t **tournament_selection(tree_t **trees,
                              const size_t nb_trees,
                              const size_t t_size) {
  tree_t **new_trees = malloc(sizeof(tree_t *) * nb_trees);

  for (int i = 0; i < nb_trees; i++) {
    /* Form tournament - keep best */
    int idx = randi(0, nb_trees);
    tree_t *best = trees[idx];
    for (int j = 1; j < t_size; j++) {
      tree_t *t = trees[randi(0, nb_trees)];
      if (t->score < best->score) {
        best = t;
      }
    }

    new_trees[i] = tree_copy(best);
  }

  return new_trees;
}

/******************************************************************************
 *                              REGRESSION
 ******************************************************************************/

int csv_rows(const char *fp) {
	/* Load file */
  FILE *infile = fopen(fp, "r");
  if (infile == NULL) {
		fclose(infile);
    return -1;
  }

	/* Loop through lines */
  int nb_rows = 0;
	char line[1024] = {0};
	size_t len_max = 1024;
  while (fgets(line, len_max, infile) != NULL) {
		if (line[0] != '#') {
			nb_rows++;
		}
  }

	/* Cleanup */
	fclose(infile);

  return nb_rows;
}

int csv_cols(const char *fp) {
	/* Load file */
  FILE *infile = fopen(fp, "r");
  if (infile == NULL) {
		fclose(infile);
    return -1;
  }

	/* Get line that isn't the header */
	char line[1024] = {0};
	size_t len_max = 1024;
  while (fgets(line, len_max, infile) != NULL) {
		if (line[0] != '#') {
			break;
		}
  }

	/* Parse line to obtain number of elements */
  int nb_elements = 1;
  int found_separator = 0;
  for (size_t i = 0; i < len_max; i++) {
    if (line[i] == ',') {
      found_separator = 1;
      nb_elements++;
    }
  }

	/* Cleanup */
	fclose(infile);

  return (found_separator) ? nb_elements : -1;
}

char **csv_fields(const char *fp, int *nb_fields) {
	/* Load file */
  FILE *infile = fopen(fp, "r");
  if (infile == NULL) {
		fclose(infile);
    return NULL;
  }

	/* Get last header line */
	char field_line[1024] = {0};
	char line[1024] = {0};
	size_t len_max = 1024;
  while (fgets(line, len_max, infile) != NULL) {
		if (line[0] != '#') {
			break;
		} else {
			strcpy(field_line, line);
		}
  }

	/* Parse fields */
	*nb_fields = csv_cols(fp);
	char **fields = malloc(sizeof(char *) * *nb_fields);
	int field_idx = 0;
	char field_name[100] = {0};

	for (int i = 0; i < strlen(field_line); i++) {
		char c = field_line[i];

		/* Ignore # and ' ' */
		if (c == '#' || c == ' ') {
			continue;
		}

		if (c == ',' || c == '\n') {
			/* Add field name to fields */
			fields[field_idx] = malloc(sizeof(char) * strlen(field_name) + 1);
			strcpy(fields[field_idx], field_name);
			memset(field_name, '\0', sizeof(char) * 100);
			field_idx++;
		} else {
			/* Append field name */
			field_name[strlen(field_name)] = c;
		}
	}

	/* Cleanup */
	fclose(infile);

	return fields;
}

double **csv_data(const char *fp, int *nb_rows, int *nb_cols) {
	/* Initialize memory for csv data */
	*nb_rows = csv_rows(fp);
	*nb_cols = csv_cols(fp);
	double **data = malloc(sizeof(double *) * *nb_cols);
	for (int i = 0; i < *nb_cols; i++) {
		data[i] = malloc(sizeof(double) * *nb_rows);
	}

	/* Load file */
  FILE *infile = fopen(fp, "r");
  if (infile == NULL) {
		fclose(infile);
    return NULL;
  }

	/* Loop through data */
	char line[1024] = {0};
	size_t len_max = 1024;
	int row_idx = 0;
	int col_idx = 0;

  while (fgets(line, len_max, infile) != NULL) {
		if (line[0] == '#') {
			continue;
		}

		char entry[100] = {0};
		for (int i = 0; i < strlen(line); i++) {
			char c = line[i];
			if (c == ' ') {
				continue;
			}

			if (c == ',' || c == '\n') {
				data[col_idx][row_idx] = strtod(entry, NULL);
				memset(entry, '\0', sizeof(char) * 100);
				col_idx++;
			} else {
				entry[strlen(entry)] = c;
			}
		}

		col_idx = 0;
		row_idx++;
  }

	/* Cleanup */
	fclose(infile);

	return data;
}

struct dataset_t {
	int nb_rows;
	int nb_cols;

	double **data;
	char **fields;
	char *predict;
} typedef dataset_t;

dataset_t *dataset_load(const char *fp, const char *predict) {
	dataset_t *ds = malloc(sizeof(dataset_t));

	/* Load data */
	ds->data = csv_data(fp, &ds->nb_rows, &ds->nb_cols);
	if (ds->data == NULL) {
		FATAL("Failed to load dataset [%s]!", fp);
	}

	/* Load fields */
	int nb_fields = 0;
	ds->fields = csv_fields(fp, &nb_fields);
	if (nb_fields != ds->nb_cols) {
		FATAL("Malformed csv field line! Number of rows != number of fields!");
	}

	/* Set field to predict */
	ds->predict = malloc(sizeof(char) * strlen(predict) + 1);
	strcpy(ds->predict, predict);

  return ds;
}

void dataset_delete(dataset_t *ds) {
	/* Free data */
	for (int i = 0; i < ds->nb_cols; i++) {
		free(ds->data[i]);
	}
	free(ds->data);

	/* Free fields */
	for (int i = 0; i < ds->nb_cols; i++) {
		free(ds->fields[i]);
	}
	free(ds->fields);

	/* Free predict */
	free(ds->predict);

	/* Free dataset itself */
	free(ds);
	ds = NULL;
}

double *dataset_expected(const dataset_t *ds) {
  int field_idx = 0;
  int field_found = 0;
	for (field_idx = 0; field_idx < ds->nb_cols; field_idx++) {
    if (strcmp(ds->fields[field_idx], ds->predict) == 0) {
      field_found = 1;
      break;
    }
  }

  if (field_found == 0) {
    return NULL;
  }

  return ds->data[field_idx];
}

void evaluate_resolve_node(const node_t *node, const dataset_t *ds, double *arg) {
  if (node->data_type == CONST) {
		/* Load constant */
		for (int i = 0; i < ds->nb_rows; i++) {
			arg[i] = node->value;
		}

  } else if (node->data_type == INPUT) {
		/* Find field */
		int found_field = 0;
		int field_idx = 0;
		for (field_idx = 0; field_idx < ds->nb_cols; field_idx++) {
			if (strcmp(ds->fields[field_idx], node->input_name) == 0) {
				found_field = 1;
				break;
			}
		}
		if (found_field == 0) {
			FATAL("Opps! Input [%s] not found in dataset!", node->input_name)
		}

		/* Load input data */
		for (int i = 0; i < ds->nb_rows; i++) {
			arg[i] = ds->data[field_idx][i];
		}


  } else if (node->data_type == EVAL) {
		/* Load eval data */
		for (int i = 0; i < ds->nb_rows; i++) {
			/* arg[i] = node->eval_data; */
		}

  } else {
    FATAL("Opps! shouldn't be here!")
  }
}

#define UNARY_FUNC(FUNC) \
  node_t *n_arg0 = stack_pop(&eval_stack); \
  evaluate_resolve_node(n_arg0, ds, arg0); \
  for (int i = 0; i < ds->nb_rows; i++) { \
    eval->eval_data[i] = FUNC(arg0[i]); \
  }

#define BINARY_OP(OPERATOR) \
  node_t *n_arg0 = stack_pop(&eval_stack); \
  node_t *n_arg1 = stack_pop(&eval_stack); \
  evaluate_resolve_node(n_arg0, ds, arg0); \
  evaluate_resolve_node(n_arg1, ds, arg1); \
  for (int i = 0; i < ds->nb_rows; i++) { \
    eval->eval_data[i] = arg0[i] OPERATOR arg1[i]; \
  }

#define BINARY_FUNC(FUNC) \
  node_t *n_arg0 = stack_pop(&eval_stack); \
  node_t *n_arg1 = stack_pop(&eval_stack); \
  evaluate_resolve_node(n_arg0, ds, arg0); \
  evaluate_resolve_node(n_arg1, ds, arg1); \
  for (int i = 0; i < ds->nb_rows; i++) { \
    eval->eval_data[i] = FUNC(arg0[i], arg1[i]); \
  }

int evaluate_tree(tree_t *t, const dataset_t *ds) {
  stack_t eq_stack;
  stack_t eval_stack;
  stack_setup(&eq_stack);
  stack_setup(&eval_stack);
  tree_stack(t, &eq_stack);

	double *arg0 = malloc(sizeof(double) * ds->nb_rows);
	double *arg1 = malloc(sizeof(double) * ds->nb_rows);
	node_t *eval = node_new_eval(ds->nb_rows);

  while (eq_stack.size != 0) {
    node_t *n = stack_pop(&eq_stack);

    if (n->type == FUNC_NODE) {
			switch (n->function) {
			case ADD: {BINARY_OP(+); break;}
			case SUB: {BINARY_OP(-); break;}
			case MUL: {BINARY_OP(*); break;}
			case DIV: {BINARY_OP(/); break;}
			case POW: {BINARY_FUNC(pow); break;}
			case EXP: {UNARY_FUNC(exp); break;}
			case LOG: {UNARY_FUNC(log); break;}
			case SIN: {UNARY_FUNC(sin); break;}
			case COS: {UNARY_FUNC(cos); break;}
			}
      stack_push(&eval_stack, eval);
    } else {
      stack_push(&eval_stack, n);
    }
  }

  /* Calculate RMSE */
  const double *predicted = eval->eval_data;
  const double *expected = dataset_expected(ds);
  const double n = ds->nb_rows;
  double err_sq = 0.0;
  for (int i = 0; i < n; i++) {
    err_sq += pow(predicted[i] - expected[i], 2);
  }
  const double rmse = sqrt(err_sq / n);

  /* Set error and score */
  t->error = rmse;
  t->score = rmse + (t->size) * 0.1;

  /* Clean up */
  free(arg0);
  free(arg1);
  node_delete(eval);

  return 0;
}


#endif
