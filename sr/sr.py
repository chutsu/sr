#!/usr/bin/env python3
import time
import math
import random
import datetime
import copy
import unittest


############################### FUNCTION SET #################################
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


############################### TERMINAL SET #################################
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
    # {"type": INPUT, "value": "y"}
];


################################## NODE ######################################
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
        idx = random.randint(0, len(fs) - 1);
        func = fs[idx];
        return Node.setup_func(func["type"], func["arity"])

    @staticmethod
    def random_term(ts):
        idx = random.randint(0, len(ts) - 1)
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
            line += "data: " + str(self.data)
        else:
            line += "Opps! Invalid node type [" + str(self.type) + "]!"

        return line


################################## TREE ######################################
class Tree:
    def __init__(self):
        self.root = None
        self.size = 0
        self.depth = 0

        self.error = 0.0
        self.score = 0.0

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

            elif (method == "GROW" and random.uniform(0, 1.0) > 0.5):
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
            if (random.uniform(0.0, 1.0) > 0.5):
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

    def _update_traverse(self, node, depth):
        if depth > self.depth:
            self.depth = depth

        if node.type == TERM_NODE:
            self.size += 1
            return

        self.size += 1
        for i in range(node.arity):
            self._update_traverse(node.children[i], depth + 1)

    def update(self):
        self.depth = 0
        self.size = 0
        self._update_traverse(self.root, self.depth)

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

    def _str_traverse(self, s, node):
        if node.type == TERM_NODE:
            s += "  " + str(node) + "\n"
            return s

        s += "  " + str(node) + "\n"
        for i in range(node.arity):
            s = self._str_traverse(s, node.children[i])

        return s

    def __str__(self):
        props_str = ""
        props_str += "tree.depth: " + str(self.depth) + "\n"
        props_str += "tree.size: " + str(self.size) + "\n"
        props_str += "tree.error: " + str(self.error) + "\n"
        props_str += "tree.score: " + str(self.score) + "\n"
        props_str += "tree.nodes:\n"

        nodes_str = ""
        nodes_str = self._str_traverse(nodes_str, self.root)

        return (props_str + nodes_str).strip()


############################## MUTATION ####################################
def _mutate_term_node(ts, node):
    for attempts in range(1000):
        new_node = Node.random_term(ts);
        if (new_node.data != node.data):
            node.data = new_node.data;
            return node;

    raise Runtime("Failed to get a different terminal!")


def _mutate_func_node(fs, node):
    for attempts in range(1000):
        index = random.randint(0, len(fs) - 1)
        func = fs[index]
        if (func["type"] != node.func and func["arity"] == node.arity):
            node.func = func["type"];
            return node;

    raise Runtime("Failed to get a function with an arity of %d!", node.arity)


def point_mutation(fs, ts, tree):
    idx = random.randint(0, tree.size - 1);
    node = tree.get_node(idx);

    if node.type == TERM_NODE:
        node = _mutate_term_node(ts, node);
    elif node.type == FUNC_NODE:
        node = _mutate_func_node(fs, node);


############################## CROSSOVER ####################################
def point_crossover(t1, t2):
    idx = random.randint(1, t1.size - 1)
    t1_node = t1.get_node(idx)

    idx = random.randint(1, t2.size - 1)
    t2_node = t2.get_node(idx)

    t1_nth_child = t1_node.nth_child
    t2_nth_child = t2_node.nth_child
    t1_node.parent.children[t1_nth_child] = t2_node
    t2_node.parent.children[t2_nth_child] = t1_node

    t1.update()
    t2.update()


############################### SELECTION ####################################
def tournament_selection(trees, t_size):
    assert(t_size > 1)
    pop_size = len(trees)
    new_gen = []

    for i in range(pop_size):
        # Create tournament
        tournament = []
        for j in range(t_size):
            idx = random.randint(0, pop_size - 1)
            tournament.append(trees[idx])

        # Select best from tournament
        best_tree = tournament[0]
        for j in range(0, len(tournament)):
            if tournament[j].score < best_tree.score:
                best_tree = tournament[j]

        # Add to new population
        new_gen.append(copy.deepcopy(best_tree))

    return new_gen


############################## POPULATION ###################################
class Population:
    def __init__(self, dataset, fs, ts, max_depth, pop_size):
        assert(dataset != None)
        assert(fs != None)
        assert(ts != None)
        assert(max_depth > 1)
        assert(pop_size >= 10)

        self.dataset = dataset
        self.fs = fs
        self.ts = ts
        self.max_depth = max_depth
        self.pop_size = pop_size

        self.trees = []
        for i in range(pop_size):
            t = Tree.generate("FULL", fs, ts, max_depth)
            self.trees.append(t)

    def eval(self):
        for i in range(len(self.trees)):
            eval_tree(self.trees[i], self.dataset)

    def best(self):
        best_tree = self.trees[0]
        for i in range(len(self.trees)):
            if self.trees[i].score < best_tree.score:
                best_tree = self.trees[i]
        return best_tree

    def __str__(self):
        s = ""
        for i in range(len(self.trees)):
            score = self.trees[i].score
            eq = self.trees[i].eq_str()
            s += "eq: %s [score: %f]\n" % (eq, score)
        return s


############################## REGRESSION ###################################
class Config:
    def __init__(self, dataset, fs, ts, **kwargs):
        # Data
        self.dataset = dataset

        # Tree settings
        self.fs = fs
        self.ts = ts
        self.max_depth = kwargs.get("max_depth", 2)

        # Evolve settings
        self.pop_size = kwargs.get("pop_size", 1000)
        self.max_iter = kwargs.get("max_iter", 500)
        self.prob_crossover = kwargs.get("prob_crossover", 0.5)
        self.prob_mutate = kwargs.get("prob_mutate", 0.2)


def _eval_resolve_node(node, dataset):
    if node.data_type == CONST:
        return [i for i in range(dataset["size"])]
    elif node.data_type == INPUT:
        return dataset["inputs"][node.data]
    elif node.data_type == EVAL:
        return node.data
    else:
        raise RuntimeError("Opps shouldn't be here!")


def eval_tree(tree, dataset):
    eq_stack = tree.stack()
    eval_stack = []

    try:
        while (len(eq_stack) != 0):
            node = eq_stack.pop()

            if node.type == FUNC_NODE:
                args = [eval_stack.pop() for i in range(node.arity)]
                result = []
                arg0 = []
                arg1 = []

                if node.func == ADD:
                    arg0 = _eval_resolve_node(args[0], dataset)
                    arg1 = _eval_resolve_node(args[1], dataset)
                    for i in range(dataset["size"]):
                        result.append(arg0[i] + arg1[i])

                elif node.func == SUB:
                    arg0 = _eval_resolve_node(args[0], dataset)
                    arg1 = _eval_resolve_node(args[1], dataset)
                    for i in range(dataset["size"]):
                        result.append(arg0[i] - arg1[i])

                elif node.func == MUL:
                    arg0 = _eval_resolve_node(args[0], dataset)
                    arg1 = _eval_resolve_node(args[1], dataset)
                    for i in range(dataset["size"]):
                        result.append(arg0[i] * arg1[i])

                elif node.func == DIV:
                    arg0 = _eval_resolve_node(args[0], dataset)
                    arg1 = _eval_resolve_node(args[1], dataset)
                    for i in range(dataset["size"]):
                        result.append(arg0[i] / arg1[i])

                elif node.func == POW:
                    arg0 = _eval_resolve_node(args[0], dataset)
                    arg1 = _eval_resolve_node(args[1], dataset)
                    for i in range(dataset["size"]):
                        result.append(math.pow(arg0[i], arg1[i]))

                elif node.func == EXP:
                    arg0 = _eval_resolve_node(args[0], dataset)
                    for i in range(dataset["size"]):
                        result.append(math.exp(arg0[i]))

                elif node.func == LOG:
                    arg0 = _eval_resolve_node(args[0], dataset)
                    for i in range(dataset["size"]):
                        result.append(math.log(arg0[i]))

                eval_stack.append(Node.setup_eval(result))

            else:
                eval_stack.append(node)

        # Calculate RMSE
        predicted = eval_stack.pop().data
        response = dataset["response"]
        n = dataset["size"]
        err_sq = 0.0
        for i in range(n):
            err_sq += math.pow(predicted[i] - response[i], 2)
        rmse = math.sqrt(err_sq / n)

        if (rmse == 0.0):
            print("predicted: %f" % predicted[0])
            print("response: %f" % response[0])
            print("error: %f" % (predicted[i] - response[i]))
            print(tree)
            exit(0)

        # Set error and score
        tree.error = rmse
        tree.score = rmse + (tree.size) * 0.1

    except Exception:
        # Set error and score
        tree.error = float("inf")
        tree.score = float("inf")
        return False

    return True


def regress(config):
    pop = Population(config.dataset,
                     config.fs,
                     config.ts,
                     config.max_depth,
                     config.pop_size)

    for i in range(config.max_iter):
        # Evaluate and select
        pop.eval()
        t_size = int(len(pop.trees) * 0.05)
        pop.trees = tournament_selection(pop.trees, t_size)

        # Crossover
        for idx in range(0, len(pop.trees), 2):
            if random.uniform(0.0, 1.0) > config.prob_crossover:
                point_crossover(pop.trees[idx], pop.trees[idx + 1]);

        # Mutate
        for t in pop.trees:
            if random.uniform(0.0, 1.0) > config.prob_mutate:
                point_mutation(fs, ts, t)

        best_tree = pop.best();

        status = ""
        status += "iter: %d " % (i)
        status += best_tree.eq_str() + " "
        status += "score: " + str(best_tree.score) + " "
        status += "error: " + str(best_tree.error) + " "
        print(status)


############################### UNIT-TESTS ###################################
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
        t1 = Tree.generate("FULL", fs, ts, 2)
        t2 = Tree.generate("FULL", fs, ts, 2)

        t1_old = copy.deepcopy(t1)
        t2_old = copy.deepcopy(t2)
        point_crossover(t1, t2)

        print(t1)
        print("-" * 78)
        print(t2)
        point_mutation(fs, ts, t1)
        point_mutation(fs, ts, t2)

        self.assertTrue(t1_old.eq_str() != t1.eq_str())
        self.assertTrue(t2_old.eq_str() != t2.eq_str())

        debug = False
        # debug = True
        if debug:
            t1_old.print_equation()
            t2_old.print_equation()
            t1_old.print_equation()
            t2_old.print_equation()

class TestSelection(unittest.TestCase):
    def test_tournament_selection(self):
        # Test
        trees = [Tree.generate("FULL", fs, ts, 2) for i in range(10)]
        t_size = 10000
        trees[0].score = -1.0 * float("inf")
        new_trees = tournament_selection(trees, t_size)

        # Assert
        for t in new_trees:
            self.assertTrue(t.score == -1.0 * float("inf"))

        # Debug
        debug = False
        if debug:
            # Old trees
            print("-" * 78)
            print("trees:")
            for t in trees:
                print("\t" + t.eq_str() + "[score: " + str(t.score) + "]")

            # New trees
            print("\nnew_trees:")
            for t in new_trees:
                print("\t" + t.eq_str() + "[score: " + str(t.score) + "]")
            print("-" * 78)

class TestPopulation(unittest.TestCase):
    def test_population(self):
        dataset = {}
        max_depth = 2
        pop_size = 10
        pop = Population(dataset, fs, ts, max_depth, pop_size)

        self.assertTrue(pop.dataset == dataset)
        self.assertTrue(pop.max_depth == max_depth)
        self.assertTrue(pop.pop_size == pop_size)
        self.assertTrue(len(pop.trees) > 0)

    def test_eval(self):
        dataset = {
            "inputs": {"x": [1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0]},
            "response": [2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0],
            "size": 10
        }
        max_depth = 2
        pop_size = 10
        pop = Population(dataset, fs, ts, max_depth, pop_size)
        pop.eval()

        for t in pop.trees:
            self.assertTrue(t.score != 0.0)

    def test_best(self):
        dataset = {
            "inputs": {"x": [1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0]},
            "response": [2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0],
            "size": 10
        }
        max_depth = 2
        pop_size = 10
        pop = Population(dataset, fs, ts, max_depth, pop_size)
        pop.eval()
        best_tree = pop.best()

        debug = False
        if debug:
            print("BEST TREE: " + str(best_tree))

class TestRegression(unittest.TestCase):
    def test_eval_tree(self):
        t_start = time.time()
        dataset = {
            "inputs": {"x": [1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0]},
            "response": [2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0],
            "size": 10
        }

        tree = Tree.generate("FULL", fs, ts, 2)
        eval_tree(tree, dataset)
        t_end = time.time()
        print(tree)

        debug = False
        if debug:
            print(tree)
            print("Time taken: %fs" % (t_end - t_start))

    def test_regress(self):
        x_data = range(10)
        y_data = [math.pow(x, 2) + 10.0 for x in x_data]

        dataset = {
            "inputs": {"x": x_data},
            "response": y_data,
            "size": 10
        }

        import pprint
        pprint.pprint(dataset)

        config = Config(dataset, fs, ts)
        regress(config)


if __name__ == "__main__":
    random.seed(datetime.datetime.now())
    unittest.main()
