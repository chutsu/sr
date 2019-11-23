/**
 * Returns a random integer between min (inclusive) and max (inclusive).
 * The value is no lower than min (or the next integer greater than min
 * if min isn't an integer) and no greater than max (or the next integer
 * lower than max if max isn't an integer).
 * Using Math.round() will give you a non-uniform distribution!
 */
function randi(min, max) {
  min = Math.ceil(min);
  max = Math.floor(max);
  return Math.floor(Math.random() * (max - min + 1)) + min;
}

/******************************************************************************
 *                              FUNCTION SET
 ******************************************************************************/

const ADD = "ADD";
const SUB = "SUB";
const MUL = "MUL";
const DIV = "DIV";
const POW = "POW";
const EXP = "EXP";
const LOG = "LOG";

const fs = [
  {"type": ADD, "arity": 2},
  {"type": SUB, "arity": 2},
  {"type": MUL, "arity": 2},
  {"type": DIV, "arity": 2},
  {"type": POW, "arity": 2},
  {"type": EXP, "arity": 1},
  {"type": LOG, "arity": 1}
];

/******************************************************************************
 *                              TERMINAL SET
 ******************************************************************************/

const CONST = 0;
const INPUT = 1;
const EVAL = 2;

const ts = [
  {"type": CONST, "value": 0.0},
  {"type": CONST, "value": 1.0},
  {"type": CONST, "value": 2.0},
  {"type": CONST, "value": 3.0},
  {"type": CONST, "value": 4.0},
  {"type": CONST, "value": 5.0},
  {"type": CONST, "value": 6.0},
  {"type": CONST, "value": 7.0},
  {"type": CONST, "value": 8.0},
  {"type": CONST, "value": 9.0},
  {"type": CONST, "value": 10.0},
  {"type": INPUT, "value": "x"},
  // {"type": INPUT, "value": "y"}
];

/******************************************************************************
 *                                   NODE
 ******************************************************************************/

const FUNC_NODE = 0;
const TERM_NODE = 1;

function node_new(){
  return {
    // General
    type: -1,
    parent: null,
    nth_child: -1,

    // Terminal node specific
    data: null,

    // Function node specific
    func: null,
    arity: -1,
    children: [],

    // Misc
    error: 0.0
  }
}

function node_str(node) {
  var line = "";

  switch (node.type) {
  case FUNC_NODE:
    switch (node.func) {
    case ADD: line += "func: ADD\t"; break;
    case SUB: line += "func: SUB\t"; break;
    case MUL: line += "func: MUL\t"; break;
    case DIV: line += "func: DIV\t"; break;
    case POW: line += "func: POW\t"; break;
    case EXP: line += "func: EXP\t"; break;
    case LOG: line += "func: LOG\t"; break;
    default:
      line += "Opps! Invalid function type[" + node.func + "]!";
      break;
    }
    line += "arity: " + node.arity;
    break;
  case TERM_NODE:
    line += "data: " + node.data;
    break;
  default:
    line += "Opps! Invalid node type [" + node.type + "]!";
    break;
  }

  return line;
}

function node_print(node) {
  var str = node_str(node);
  console.log(str);
}

function node_setup_func(func, arity) {
  n = node_new();
  n.type = FUNC_NODE;
  n.func = func;
  n.arity = arity;
  return n;
}

function node_setup_input(input_name) {
  n = node_new();
  n.type = TERM_NODE;
  n.data_type = INPUT;
  n.data = input_name;
  return n;
}

function node_setup_const(value) {
  n = node_new();
  n.type = TERM_NODE;
  n.data_type = CONST;
  n.data = value;
  return n;
}

function node_setup_eval(value) {
  n = node_new();
  n.type = TERM_NODE;
  n.data_type = EVAL;
  n.data = value;
  return n;
}

function random_func(fs) {
  var idx = randi(0, fs.length - 1);
  var func = fs[idx];
  return node_setup_func(func.type, func.arity);
}

function random_term(ts) {
  var idx = randi(0, ts.length - 1);
  var term = ts[idx];

  switch (term.type) {
  case INPUT: return node_setup_input(term.value);
  case CONST: return node_setup_const(term.value);
  }
}

/******************************************************************************
 *                                   TREE
 ******************************************************************************/

function tree_new() {
  return {
    "root": null,
    "size": 0,
    "depth": 0,

    "error": 0.0,
    "score": 0.0,
    "hits": 0
  }
}

function tree_print_traverse(str, node) {
  if (node.type == TERM_NODE) {
    str += "  " + node_str(node) + "\n";
    return str;
  }

  str += "  " + node_str(node) + "\n";
  for (var i = 0; i < node.arity; i++) {
    str = tree_print_traverse(str, node.children[i]);
  }

  return str;
}

function tree_print(tree) {
  var props_str = "";
  props_str += "tree.depth: " + t.depth + "\n";
  props_str += "tree.size: " + t.size + "\n";
  props_str += "tree.error: " + t.error + "\n";
  props_str += "tree.score: " + t.score + "\n";
  props_str += "tree.hits: " + t.hits + "\n";
  props_str += "tree.nodes:\n";

  var nodes_str = "";
  var nodes_str = tree_print_traverse(nodes_str, t.root);
  console.log(props_str + nodes_str);
}

const FULL = 0;
const GROW = 1;
const RAMPED_HALF_AND_HALF = 2;

function tree_build(method, tree, node, fs, ts, curr_depth, max_depth) {
  // Pre-check
  if (curr_depth > tree.depth) {
    tree.depth++;
  }

  // Build - Depth first traversal manner
  for (var i = 0; i < node.arity; i++) {
    if (curr_depth == max_depth) {
      // Create terminal node
      child = random_term(ts);
      child.parent = node;
      child.nth_child = i;
      node.children[i] = child;
      tree.size++;

    } else if (method == GROW && randf(0, 1.0) > 0.5) {
      // Create terminal node
      child = random_term(ts);
      child.parent = node;
      child.nth_child = i;
      node.children[i] = child;
      tree.size++;

    } else {
      // Create function node
      child = random_func(fs);
      child.parent = node;
      child.nth_child = i;
      node.children[i] = child;
      tree.size++;

      // Recurse deeper
      tree_build(method, tree, child, fs, ts, curr_depth + 1, max_depth);
    }
  }
}

function tree_generate(method, fs, ts, max_depth) {
  t = tree_new();
  t.root = random_func(fs);
  t.size++;
  t.depth++;

  switch (method) {
    case FULL:
    case GROW:
      tree_build(method, t, t.root, fs, ts, 1, max_depth);
      break;
    case RAMPED_HALF_AND_HALF:
      if (randf(0.0, 1.0) > 0.5) {
        tree_build(FULL, t, t.root, fs, ts, 1, max_depth);
      } else {
        tree_build(GROW, t, t.root, fs, ts, 1, max_depth);
      }
      break;
    default:
      return -1;
  }

  return t;
}

function tree_traverse(stack, node) {
  if (node.type == TERM_NODE) {
    stack.push(node);
    return stack;
  }

  stack.push(node);
  for (var i = 0; i < node.arity; i++) {
    stack = tree_traverse(stack, node.children[i]);
  }

  return stack;
}

function tree_stack(tree) {
  var stack = [];
  return tree_traverse(stack, tree.root);
}

function tree_get_node(tree, idx) {
  var stack = tree_stack(tree);
  return stack[idx];
}

function tree_equation_str_traverse(str, node) {
  if (node.type == TERM_NODE) {
    str += node.data + " ";
    return str;
  }

  str += node.func + " ";
  for (var i = 0; i < node.arity; i++) {
    str = tree_equation_str_traverse(str, node.children[i]);
  }

  return str;
}

function tree_equation_str(tree) {
  var str = "";
  str = tree_equation_str_traverse(str, tree.root);
  return str;
}

function tree_print_equation(tree) {
  console.log(tree_equation_str(tree));
}

/******************************************************************************
 *                                 MUTATION
 ******************************************************************************/

function mutate_term_node(ts, node) {
  for (var attempts = 0; attempts < 1000; attempts++) {
    new_node = random_term(ts);
    if (new_node.data !== node.data) {
      node.data = new_node.data;
      return node;
    }
  }

  return null;
}

function mutate_func_node(fs, node) {
  for (var attempts = 0; attempts < 1000; attempts++) {
    var idx = randi(0, fs.length - 1);
    var func = fs[idx];
    if (func.type !== node.func && func.arity == node.arity) {
      node.func = func.type;
      return node;
    }
  }
  console.log("Failed to get a function with an arity of %d!", node.arity);

  return null;
}

function point_mutation(fs, ts, tree) {
  var idx = randi(0, tree.size - 1);
  node = tree_get_node(t, idx);

  if (node.type == TERM_NODE) {
    node = mutate_term_node(ts, node);
  } else if (node.type == FUNC_NODE) {
    node = mutate_func_node(fs, node);
  }
}

/******************************************************************************
 *                                CROSSOVER
 ******************************************************************************/

function point_crossover(t1, t2) {
  var idx = randi(1, t1.size - 1);
  t1_node = tree_get_node(t1, idx);

  var idx = randi(1, t2.size - 1);
  t2_node = tree_get_node(t2, idx);

  var t1_nth_child = t1_node.nth_child;
  var t2_nth_child = t2_node.nth_child;
  t1_node.parent.children[t1_nth_child] = t2_node;
  t2_node.parent.children[t2_nth_child] = t1_node;
}

/******************************************************************************
 *                                REGRESSION
 ******************************************************************************/

function eval_resolve_node(node, dataset) {
  switch (node.data_type) {
  case CONST:
    var data = [];
    for (var i = 0; i < dataset.size; i++) {
      data.push(node.data);
    }
    return data;
  case INPUT:
    return dataset["inputs"][node.data];
  case EVAL:
    return node.data;
  default:
    return null;
  }
}

function eval_tree(tree, dataset) {
  var eq_stack = tree_stack(tree);
  var eval_stack = [];

  while (eq_stack.length != 0) {
    var node = eq_stack.pop();

    if (node.type == FUNC_NODE) {
      var args = [];
      for (var i = 0; i < node.arity; i++) {
        args.push(eval_stack.pop());
      }

      var result = [];
      var arg0 = [];
      var arg1 = [];
      switch (node.func) {
      case ADD:
        arg0 = eval_resolve_node(args[1], dataset);
        arg1 = eval_resolve_node(args[0], dataset);
        for (var i = 0; i < dataset.size; i++) {
          result.push(arg0[i] + arg1[i]);
        }
        break;
      case SUB:
        arg0 = eval_resolve_node(args[1], dataset);
        arg1 = eval_resolve_node(args[0], dataset);
        for (var i = 0; i < dataset.size; i++) {
          result.push(arg0[i] - arg1[i]);
        }
        break;
      case MUL:
        arg0 = eval_resolve_node(args[1], dataset);
        arg1 = eval_resolve_node(args[0], dataset);
        for (var i = 0; i < dataset.size; i++) {
          result.push(arg0[i] * arg1[i]);
        }
        break;
      case DIV:
        arg0 = eval_resolve_node(args[1], dataset);
        arg1 = eval_resolve_node(args[0], dataset);
        for (var i = 0; i < dataset.size; i++) {
          result.push(arg0[i] / arg1[i]);
        }
        break;
      case POW:
        arg0 = eval_resolve_node(args[1], dataset);
        arg1 = eval_resolve_node(args[0], dataset);
        for (var i = 0; i < dataset.size; i++) {
          result.push(Math.pow(arg0[i], arg1[i]));
        }
        break;
      case EXP:
        arg0 = eval_resolve_node(args[0], dataset);
        for (var i = 0; i < dataset.size; i++) {
          result.push(Math.exp(arg0[i]));
        }
        break;
      case LOG:
        arg0 = eval_resolve_node(args[0], dataset);
        for (var i = 0; i < dataset.size; i++) {
          result.push(Math.log(arg0[i]));
        }
        break;
      }
      eval_stack.push(node_setup_eval(result));

    } else {
      eval_stack.push(node);
    }
  }

  return eval_stack.pop().data;
}

/******************************************************************************
 *                                   TEST
 ******************************************************************************/

function test_point_mutation() {
  for (var i = 0; i < 100; i++) {
    tree = tree_generate(FULL, fs, ts, 2);
    tree_print_equation(tree);

    point_mutation(fs, ts, tree);
    tree_print_equation(tree);
  }
}

function test_point_crossover() {
  t1 = tree_generate(FULL, fs, ts, 2);
  t2 = tree_generate(FULL, fs, ts, 2);
  tree_print_equation(t1);
  tree_print_equation(t2);

  point_crossover(t1, t2);
  tree_print_equation(t1);
  tree_print_equation(t2);
}

function test_eval_tree() {
  console.time("eval_tree");
  data = {
    "inputs": {
      "x": [1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0],
    },
    "response": {
      "y": [2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0],
    },
    "size": 10
  };

  tree = tree_generate(FULL, fs, ts, 2);
  result = eval_tree(tree, data);
  tree_print_equation(tree);
  console.log(result);

  // var workers = [];
  // workers.push(new Worker("sr_worker.js"));
  // for (var i = 0; workers.length; i++) {
  //   w.onmessage = function(event) {
  //     console.log(event.data);
  //   };
  // }

  console.timeEnd("eval_tree");
}

// test_point_mutation();
// test_point_crossover();
test_eval_tree();
