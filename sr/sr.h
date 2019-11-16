#ifndef SR_H
#define SR_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "sr/utils.h"

/* PARAMETERS */
#define MAX_ARITY 10
#define MAX_TREE_SIZE 500

/******************************************************************************
 * 														   FUNCTION SET
 ******************************************************************************/

/* Function Types */
#define ADD 0
#define SUB 1
#define MUL 2
#define DIV 3
#define POW 4
#define EXP 5
#define LOG 6

/* Function Arity */
#define ADD_ARITY 2
#define SUB_ARITY 2
#define MUL_ARITY 2
#define DIV_ARITY 2
#define POW_ARITY 2
#define EXP_ARITY 1
#define LOG_ARITY 1

typedef struct function_set_t {
  int *funcs;
  int *arity;
  int length;
} function_set_t;

void function_set_print(const function_set_t *fs) {
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
    }
  }
  printf("\n");
}

/******************************************************************************
 * 														   TERMINAL SET
 ******************************************************************************/

/* Terminal Types */
#define INPUT 0
#define CONST 1
#define RCONST 2
#define EVAL 3

typedef struct terminal_t {
  int type;
  union {
    const char *str;
    double val;
    double range[2];
  };
} terminal_t;

typedef struct terminal_set_t {
  terminal_t *terms;
  int length;
} terminal_set_t;

void terminal_set_print(const terminal_set_t *ts) {
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
 * 																	NODE
 ******************************************************************************/

/* Node Types */
#define FUNC_NODE 0
#define TERM_INPUT_NODE 1
#define TERM_CONST_NODE 2
#define TERM_EVAL_NODE 3

typedef struct node_t {
  /* General */
  int type;
	struct node_t *parent;
	int nth_child;

  /* Terminal node specific */
	double value;
	const char *input_name;

  /* Function node specific */
	int function;
	int arity;
	struct node_t *children[MAX_ARITY];

	/* Misc */
	double error;
} node_t;

void node_setup(node_t *n) {
	/* General */
	n->type = -1;
	n->parent = NULL;
	n->nth_child = -1;

	/* Terminal node specific */
	n->value = 0.0;

	/* Function node specific */
	n->function = -1;
	n->arity = -1;
	for (int i = 0; i < MAX_ARITY; i++) {
		n->children[i] = NULL;
	}

	/* Evaluation */
	n->error = 0.0;
}

void node_clear(node_t *n) {
  node_setup(n);
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
    default: printf("Opps! Invalid function type[%d]!", n->function); break;
    }
    printf("arity: %d\n", n->arity);
    break;
  case TERM_INPUT_NODE:
    printf("input\t");
    printf("input_name: %s\n", n->input_name);
    break;
  case TERM_CONST_NODE:
    printf("const\t");
    printf("value: %f\n", n->value);
    break;
  default:
    printf("Opps! Invalid node type [%d]!", n->type);
    break;
  }
}

void node_setup_func(node_t *n, const int function, const int arity) {
	node_setup(n);
	n->type = FUNC_NODE;
	n->function = function;
	n->arity = arity;
}

void node_setup_input(node_t *n, const char *input_name) {
	node_setup(n);
	n->type = TERM_INPUT_NODE;
	n->input_name = input_name;
}

void node_setup_const(node_t *n, double value) {
	node_setup(n);
	n->type = TERM_CONST_NODE;
	n->value = value;
}

/* void node_setup_eval(node_t *n, double *values, size_t n_values) { */
/* 	node_setup(n, TERM_EVAL_NODE); */
/* 	#<{(| n->n_values = n_values; |)}># */
/* 	#<{(| n->values = values; |)}># */
/* } */

void random_func(const function_set_t *fs, node_t *n) {
	const int idx = randi(0, fs->length);
	node_setup_func(n, fs->funcs[idx], fs->arity[idx]);
}

void random_term(const terminal_set_t *ts, node_t *n) {
	assert(ts->length > 1);
	const int idx = randi(0, ts->length);
	const terminal_t *term = &ts->terms[idx];

	node_setup(n);
	switch (term->type) {
	case INPUT:
		node_setup_input(n, term->str);
		break;
	case CONST:
		node_setup_const(n, term->val);
		break;
	case RCONST:
		node_setup_const(n, randf(term->range[0], term->range[1]));
		break;
	}
}

/******************************************************************************
 * 																	TREE
 ******************************************************************************/

typedef struct tree_t {
  node_t nodes[MAX_TREE_SIZE];
  node_t *root;
  int size;
  int depth;

  double error;
  double score;
  int hits;
} tree_t;

void tree_setup(tree_t *t) {
  for (int i = 0; i < MAX_TREE_SIZE; i++) {
    node_clear(&t->nodes[i]);
  }
  t->root = &t->nodes[0];
  t->size = 0;
  t->depth = 0;

  t->error = 0.0;
  t->score = 0.0;
  t->hits = 0.0;
}

void tree_clear(tree_t *t) {
  tree_setup(t);
}

void tree_print(const tree_t *t) {
  printf("----------------------------------------\n");
  printf("tree.depth: %d\n", t->depth);
  printf("tree.size: %d\n", t->size);
  printf("tree.error: %f\n", t->error);
  printf("tree.score: %f\n", t->score);
  printf("tree.hits: %d\n", t->hits);
  printf("\n");

  printf("tree.nodes:\n");
  for (int i = 0; i < t->size; i++) {
    printf("  ");
    node_print(&t->nodes[i]);
  }
  printf("----------------------------------------\n");
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
			node_t *child = &t->nodes[t->size];
			random_term(ts, child);
			child->parent = n;
			child->nth_child = i;
			n->children[i] = child;
			t->size++;

		} else if (method == GROW && randf(0, 1.0) > 0.5) {
			/* Create terminal node */
			node_t *child = &t->nodes[t->size];
			random_term(ts, child);
			child->parent = n;
			child->nth_child = i;
			n->children[i] = child;
			t->size++;

		} else {
			/* Create function node */
			node_t *child = &t->nodes[t->size];
			random_func(fs, child);
			child->parent = n;
			child->nth_child = i;
			n->children[i] = child;
      t->size++;

      /* Recurse deeper */
			tree_build(method, t, child, fs, ts, curr_depth + 1, max_depth);
		}
	}
}

int tree_generate(const int method,
							    const function_set_t *fs,
							    const terminal_set_t *ts,
							    const int max_depth,
							    tree_t *t) {
	/* Pre-check */
	assert(method > -1);
	assert(fs != NULL);
	assert(ts != NULL);
	assert(max_depth > 0);
	assert(t != NULL);

	/* Generate tree */
	tree_setup(t);
	random_func(fs, t->root);
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
			return -1;
	}
	/* tree_update(tree); */

	return 0;
}

static void tree_update_traverse(tree_t *t, node_t *n, const int depth) {
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
	/* Reset size and depth */
	t->depth = 0;
	t->size = 0;

	/* Update tree */
	tree_update_traverse(t, t->root, 0);
}

static int subtree_size_traverse(const node_t *n, int size) {
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
 * 													  MUTATION OPERATORS
 ******************************************************************************/

void point_mutation(const function_set_t *fs,
										const terminal_set_t *ts,
										tree_t *t) {
  const int idx = randi(0, t->size);
	node_t *n = &t->nodes[idx];

	if (n->type == TERM_INPUT_NODE || n->type == TERM_CONST_NODE) {
		struct node_t *parent = n->parent;
	  const int nth_child = n->nth_child;
		random_term(ts, n);
		n->parent = parent;
		n->nth_child = nth_child;

	} else if (n->type == FUNC_NODE) {
		int attempts = 0;
rand_func:
		{
			int i = randi(0, fs->length);
			if (fs->arity[i] != n->arity && attempts < 20) {
				goto rand_func;
			} else if (attempts >= 20) {
				FATAL("Failed to get a function with an arity of %d!", n->arity);
			}
			n->function = fs->funcs[i];
		}

	}
}

/******************************************************************************
 * 													  CROSSOVER OPERATORS
 ******************************************************************************/

#endif
