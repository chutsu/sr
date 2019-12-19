#!/usr/bin/env python3
from math import ceil
from math import floor
from math import pow
from math import sqrt
from random import seed
from random import randint
from random import uniform
import unittest


# FUNCTION SET
ADD = "ADD"
SUB = "SUB"
MUL = "MUL"
DIV = "DIV"
POW = "POW"
EXP = "EXP"
LOG = "LOG"

fs = [
    {"type": "ADD", "arity": 2},
    {"type": "SUB", "arity": 2},
    {"type": "MUL", "arity": 2},
    {"type": "DIV", "arity": 2},
    {"type": "POW", "arity": 2},
    {"type": "EXP", "arity": 1},
    {"type": "LOG", "arity": 1}
]

# TERMINAL SET
CONST = 0
INPUT = 1
EVAL = 2

ts = [
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
    {"type": INPUT, "value": "y"}
];

# NODE
FUNC_NODE = 0
TERM_NODE = 1

class Node:
    def __init__(self):
        # General
        self.type =  -1
        self.parent = None
        self.nth_child = -1

        # Terminal node specific
        self.data = None

        # Function node specific
        self.func = None
        self.arity = -1
        self.children = []

        # Misc
        self.error = 0.0

    @staticmethod
    def setup_func(func, arity):
        n = Node();
        n.type = FUNC_NODE;
        n.func = func;
        n.arity = arity;
        n.children = [-1 for i in range(arity)]
        return n;

    @staticmethod
    def setup_input(input_name):
        n = Node()
        n.type = TERM_NODE
        n.data_type = INPUT
        n.data = input_name
        return n

    @staticmethod
    def setup_const(value):
        n = Node()
        n.type = TERM_NODE
        n.data_type = CONST
        n.data = value
        return n

    @staticmethod
    def setup_eval(value):
        n = Node()
        n.type = TERM_NODE
        n.data_type = EVAL
        n.data = value
        return n

    @staticmethod
    def random_func(fs):
        idx = randint(0, len(fs) - 1);
        func = fs[idx];
        return Node.setup_func(func["type"], func["arity"])

    @staticmethod
    def random_term(ts):
        idx = randint(0, len(ts) - 1)
        term = ts[idx]

        if term["type"] == INPUT:
            return Node.setup_input(term["value"])
        elif term["type"] == CONST:
            return Node.setup_const(term["value"])

    def __str__(self):
        line = ""
        if self.type == FUNC_NODE:
            if self.func == ADD: line += "func: ADD\t"
            elif self.func == SUB: line += "func: SUB\t"
            elif self.func == MUL: line += "func: MUL\t"
            elif self.func == DIV: line += "func: DIV\t"
            elif self.func == POW: line += "func: POW\t"
            elif self.func == EXP: line += "func: EXP\t"
            elif self.func == LOG: line += "func: LOG\t"
            else:
                line += "Opps! Invalid def type[" + self.func + "]!"
            line += "arity: " + str(self.arity)
        elif self.type == TERM_NODE:
            line += "data: " + self.data
        else:
            line += "Opps! Invalid node type [" + str(self.type) + "]!"
        return line


# TREE
class Tree:
    def __init__(self):
        self.root = None
        self.size = 0
        self.depth = 0

        self.error = 0.0
        self.score = 0.0

    def _print_traverse(self, s, node):
        if node.type == TERM_NODE:
            s += "  " + str(node) + "\n"
            return s

        s += "  " + str(node) + "\n"
        for i in range(node.arity):
            s = self._print_traverse(s, node.children[i])

        return s

    # def print(self):
    #     props_str = ""
    #     props_str += "tree.depth: " + self.depth + "\n"
    #     props_str += "tree.size: " + self.size + "\n"
    #     props_str += "tree.error: " + self.error + "\n"
    #     props_str += "tree.score: " + self.score + "\n"
    #     props_str += "tree.hits: " + self.hits + "\n"
    #     props_str += "tree.nodes:\n"
    #
    #     nodes_str = ""
    #     nodes_str = self._print_traverse(nodes_str, self.root)
    #     print(props_str + nodes_str)

    @staticmethod
    def _build(method, tree, node, fs, ts, curr_depth, max_depth):
        # Pre-check
        if curr_depth > tree.depth:
            tree.depth += 1

        # Build - Depth first traversal manner
        for i in range(node.arity):
            if curr_depth == max_depth:
                # Create terminal node
                child = Node.random_term(ts)
                child.parent = node
                child.nth_child = i
                node.children[i] = child
                tree.size += 1

            elif (method == "GROW" and uniform(0, 1.0) > 0.5):
                # Create terminal node
                child = Node.random_term(ts)
                child.parent = node
                child.nth_child = i
                node.children[i] = child
                tree.size += 1

            else:
                # Create function node
                child = Node.random_func(fs)
                child.parent = node
                child.nth_child = i
                node.children[i] = child
                tree.size += 1

            # Recurse deeper
            Tree._build(method, tree, child, fs, ts, curr_depth + 1, max_depth)

        return tree

    @staticmethod
    def generate(method, fs, ts, max_depth):
        t = Tree()
        t.root = Node.random_func(fs)
        t.size += 1
        t.depth += 1

        if method == "FULL" or method == "GROW":
            return Tree._build(method, t, t.root, fs, ts, 1, max_depth)
        elif method == "RAMPED_HALF_AND_HALF":
            if (uniform(0.0, 1.0) > 0.5):
                return Tree._build("FULL", t, t.root, fs, ts, 1, max_depth);
            else:
                return Tree._build("GROW", t, t.root, fs, ts, 1, max_depth);
        else:
            return None

    def _stack_traverse(self, stack, node):
        if node.type == TERM_NODE:
            stack.append(node)
            return stack

        stack.append(node)
        for i in range(node.arity):
            stack = self._stack_traverse(stack, node.children[i])

        return stack

    def stack(self):
        stack = [];
        return self._stack_traverse(stack, self.root)

    def get_node(self, index):
        stack = self.stack()
        return stack[index]

    def _eq_str_traverse(self, s, node):
        if node.type == TERM_NODE:
            s += str(node.data) + " "
            return s

        s += node.func + " ";
        for i in range(node.arity):
            s = self._eq_str_traverse(s, node.children[i])

        return s

    def eq_str(self):
        s = ""
        return self._eq_str_traverse(s, self.root)

    def print_equation(self):
        print(self.eq_str())


# MUTATION
def _mutate_term_node(ts, node):
    for attempts in range(1000):
        new_node = Node.random_term(ts);
        if (new_node.data != node.data):
            node.data = new_node.data;
            return node;

    raise Runtime("Failed to get a different terminal!")


def _mutate_func_node(fs, node):
    for attempts in range(1000):
        index = randint(0, len(fs) - 1)
        func = fs[index]
        if (func["type"] != node.func and func["arity"] == node.arity):
            node.func = func["type"];
            return node;

    raise Runtime("Failed to get a function with an arity of %d!", node.arity)


def point_mutation(fs, ts, tree):
    idx = randint(0, tree.size - 1);
    node = tree.get_node(idx);

    if node.type == TERM_NODE:
        node = _mutate_term_node(ts, node);
    elif node.type == FUNC_NODE:
        node = _mutate_func_node(fs, node);


# CROSSOVER

def point_crossover(t1, t2):
    idx = randint(1, t1.size - 1)
    t1_node = t1.get_node(idx)

    idx = randint(1, t2.size - 1)
    t2_node = t2.get_node(idx)

    t1_nth_child = t1_node.nth_child
    t2_nth_child = t2_node.nth_child
    t1_node.parent.children[t1_nth_child] = t2_node
    t2_node.parent.children[t2_nth_child] = t1_node


# REGRESSION
def _eval_resolve_node(node, dataset):
    if node.data_type == CONST:
        return [i for i in range(dataset.size)]
    elif node.data_type == INPUT:
        return dataset["inputs"][node.data]
    elif node.data_type == EVAL:
        return node.data
    else:
        raise RuntimeError("Opps shouldn't be here!")


def eval_tree(tree, dataset):
    eq_stack = tree.stack()
    eval_stack = []

    while (len(eq_stack) != 0):
        node = eq_stack.pop()

        if node.type == FUNC_NODE:
            args = []
            for i in range(node.arity):
                args.push(eval_stack.pop())

            result = []
            arg0 = []
            arg1 = []

            if node.func == ADD:
                arg0 = _eval_resolve_node(args[1], dataset)
                arg1 = _eval_resolve_node(args[0], dataset)
                for i in range(dataset.size):
                    result.append(arg0[i] + arg1[i])

            elif node.func == SUB:
                arg0 = _eval_resolve_node(args[1], dataset)
                arg1 = _eval_resolve_node(args[0], dataset)
                for i in range(dataset.size):
                    result.append(arg0[i] - arg1[i])

            elif node.func == MUL:
                arg0 = _eval_resolve_node(args[1], dataset)
                arg1 = _eval_resolve_node(args[0], dataset)
                for i in range(dataset.size):
                    result.append(arg0[i] * arg1[i])

            elif node.func == DIV:
                arg0 = _eval_resolve_node(args[1], dataset)
                arg1 = _eval_resolve_node(args[0], dataset)
                for i in range(dataset.size):
                    result.append(arg0[i] / arg1[i])

            elif node.func == POW:
                arg0 = _eval_resolve_node(args[1], dataset)
                arg1 = _eval_resolve_node(args[0], dataset)
                for i in range(dataset.size):
                    result.append(pow(arg0[i], arg1[i]))

            elif node.func == EXP:
                arg0 = _eval_resolve_node(args[0], dataset)
                for i in range(dataset.size):
                    result.append(exp(arg0[i]))

            elif node.func == LOG:
                arg0 = _eval_resolve_node(args[0], dataset)
                for i in range(dataset.size):
                    result.append(log(arg0[i]))

            eval_stack.append(Node.setup_eval(result))

        else:
            eval_stack.append(node)

    # Calculate RMSE
    predicted = eval_stack.pop().data
    response = dataset.response
    err_sq = 0.0
    for i in range(predicted.length):
        err_sq += pow(predicted[i] - response[i], 2)
    rmse = sqrt(err_sq / predicted.length)

    # Set error and score
    tree.error = rmse
    tree.score = rmse


# GENETIC PROGRAMMING
# def population_new(dataset, fs, ts, max_depth, pop_size) {
#   console.assert(dataset != None, {dataset});
#   console.assert(fs != None, {fs});
#   console.assert(ts != None, {ts});
#   console.assert(max_depth > 1, {max_depth});
#   console.assert(pop_size >= 10, {pop_size});
#
#   var pop = {
#     "trees": [],
#
#     "dataset": dataset,
#     "fs": fs,
#     "ts": ts,
#
#     "pop_size": 10,
#     "t_size": 5,
#   };
#
#   for (var i = 0; i < pop_size; i++) {
#     var tree = tree_generate(FULL, fs, ts, max_depth);
#     pop.trees.push(tree);
#   }
#
#   return pop;
# }
#
# def population_print(pop) {
#   for (var i = 0; i < pop.trees.length; i++) {
#     var score = pop.trees[i].score.toFixed(4);
#     var eq = tree_equation_str(pop.trees[i]);
#     console.log("eq: %s [score: %s]", eq, score.toString(10).padStart(4));
#   }
# }
#
# def population_eval(pop, dataset) {
#   console.assert(pop != None, {pop});
#   console.assert(dataset != None, {dataset});
#
#   for (var i = 0; i < pop.trees.length; i++) {
#     eval_tree(pop.trees[i], dataset);
#   }
# }
#
# def population_best(pop) {
#   var best_tree = pop.trees[0];
#
#   for (var i = 1; i < pop.trees.length; i++) {
#     if (pop.trees[i].score < best_tree.score) {
#       best_tree = pop.trees[i];
#     }
#   }
#
#   return best_tree;
# }
#
#
# def tournament_selection(pop, t_size) {
#   console.assert(t_size > 1, {t_size});
#
#   var new_pop = [];
#
#   for (var j = 0; j < pop.trees.length; j++) {
#     // Create tournament
#     var tournament = [];
#     for (var i = 0; i < t_size; i++) {
#       var idx = randint(0, pop.trees.length);
#       tournament.push(pop.trees[idx]);
#     }
#
#     // Select best from tournament
#     var best_tree = tournament[0];
#     for (var i = 1; i < t_size; i++) {
#       console.log(tournament[i]);
#       console.log(best_tree);
#       if (tournament[i].score < best_tree.score) {
#         best_tree = tournament[i];
#       }
#     }
#
#     // Add to new population
#     new_pop.push(best_tree);
#   }
#
#   pop.trees = new_pop;
# }
#
# def evolve(config) {
#   // console.log(config.fs);
#   // console.log(config.ts);
#   // console.log(config.max_depth);
#   // console.log(config.pop_size);
#   // console.log(pop);
#   // console.log(config);
#   var pop = population_new(config.dataset,
#                            config.fs,
#                            config.ts,
#                            config.max_depth,
#                            config.pop_size);
#
#   // for (var gen = 0; gen < config.max_iter; gen++) {
#     // Evaluate and select
#     population_eval(pop, config.dataset);
#     tournament_selection(pop, config.t_size);
#     console.log(pop);
#
#     // // Crossover
#     // for (var i = 0; i < config.pop_size; i+=2) {
#     //   if (uniform(0.0, 1.0) > 0.2) {
#     //     console.log(i);
#     //     console.log(pop.trees[i]);
#     //     console.log(pop.trees[i + 1]);
#     //     point_crossover(pop.trees[i], pop.trees[i + 1]);
#     //   }
#     // }
#
#     // // Mutate
#     // for (var i = 0; i < config.pop_size; i++) {
#     //   if (uniform(0.0, 1.0) > 0.02) {
#     //     point_mutation(fs, ts, pop.trees[i]);
#     //   }
#     // }
#
#   //   var best_tree = population_best(pop);
#   //   tree_print_equation(best_tree);
#   //   console.log("score: ", best_tree.score);
#   //   console.log();
#   // }
# }


# TEST
class TestNode(unittest.TestCase):
    def test_node(self):
        n = Node()
        self.assertTrue(n.type == -1)
        self.assertTrue(n.parent is None)
        self.assertTrue(n.nth_child == -1)
        self.assertTrue(n.data is None)
        self.assertTrue(n.func is None)
        self.assertTrue(n.arity == -1)
        self.assertTrue(n.children == [])
        self.assertTrue(n.error == 0.0)

    def test_setup_func(self):
        n = Node.setup_func(ADD, 2)
        self.assertTrue(n.type == FUNC_NODE)
        self.assertTrue(n.func == ADD)
        self.assertTrue(n.arity == 2)
        self.assertTrue(len(n.children) == 2)

    def test_setup_input(self):
        n = Node.setup_input("x")
        self.assertTrue(n.type == TERM_NODE)
        self.assertTrue(n.data_type == INPUT)
        self.assertTrue(n.data == "x")

    def test_setup_const(self):
        n = Node.setup_const(1.0)
        self.assertTrue(n.type == TERM_NODE)
        self.assertTrue(n.data_type == CONST)
        self.assertTrue(n.data == 1.0)

    def test_setup_eval(self):
        n = Node.setup_eval(range(10))
        self.assertTrue(n.type == TERM_NODE)
        self.assertTrue(n.data_type == EVAL)
        self.assertTrue(n.data == range(10))

    def test_random_func(self):
        n = Node.random_func(fs)
        self.assertTrue(n.type == FUNC_NODE)
        self.assertTrue(n.arity in [1, 2])

    def test_random_term(self):
        n = Node.random_term(ts)
        self.assertTrue(n.type == TERM_NODE)
        self.assertTrue(n.data_type in [INPUT, CONST])

class TestMutation(unittest.TestCase):
    def test_point_mutation(self):
        for i in range(10):
            tree = Tree.generate("FULL", fs, ts, 2);
            x = tree.eq_str()

            point_mutation(fs, ts, tree)
            y = tree.eq_str()

            self.assertTrue(x != y)

class TestCrossover(unittest.TestCase):
    def test_point_crossover(self):
        seed(0)
        t1 = Tree.generate("FULL", fs, ts, 2)
        t2 = Tree.generate("FULL", fs, ts, 2)
        t1.print_equation()
        t2.print_equation()

        point_crossover(t1, t2)
        t1.print_equation()
        t2.print_equation()

class TestRegression(unittest.TestCase):
    def test_eval_tree(self):
        pass
        # console.time("eval_tree");
        # data = {
        #     "inputs": {"x": [1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0]},
        #     "response": [2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0],
        #     "size": 10
        # };
        #
        # tree = tree_generate("FULL", fs, ts, 2);
        # eval_tree(tree, data);
        # tree_print_equation(tree);


# def test_population_new() {
#   var dataset = {};
#   var max_depth = 2;
#   var pop_size = 10;
#   var population = population_new(dataset, fs, ts, max_depth, pop_size);
#   console.log(population);
# }
#
# def test_population_eval() {
#   var dataset = {
#     "inputs": {"x": [1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0]},
#     "response": [2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0],
#     "size": 10
#   };
#   var tree_size = 2;
#   var pop_size = 10;
#   var pop = population_new(dataset, fs, ts, tree_size, pop_size);
#
#   population_eval(pop, dataset);
#   console.log(pop);
# }
#
# def test_population_best() {
#   var dataset = {
#     "inputs": {"x": [1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0]},
#     "response": [2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0],
#     "size": 10
#   };
#   var tree_size = 2;
#   var pop_size = 10;
#   var pop = population_new(dataset, fs, ts, tree_size, pop_size);
#
#   population_eval(pop, dataset);
#   // best = population_best(pop);
#   console.log(pop);
#   // tree_print(best);
# }
#
# def test_evolve() {
#   var config = {
#     "dataset": {
#       "inputs": {"x": [1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0]},
#       "response": [2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0],
#       "size": 10
#     },
#
#     "size": 100,
#     "fs": fs,
#     "ts": ts,
#
#     "max_depth": 2,
#     "pop_size": 100,
#
#     "t_size": 5,
#
#     "max_iter": 100,
#   };
#   evolve(config);
# }


unittest.main()

# // test_point_mutation();
# // test_point_crossover();
# // test_eval_tree();
# // test_population_new();
# // test_population_eval();
# // test_population_best();
# test_evolve();
