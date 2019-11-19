function eval_resolve_node(node, inputs) {
  switch (node.data_type) {
  case CONST: return node.data;
  case EVAL: return inputs[node.data];
  default: return null;
  }
}

function eval_tree(tree, inputs) {
  var eq_stack = tree_stack(tree);
  var eval_stack = [];

  while (eq_stack.length != 0) {
    var node = eq_stack.pop();

    if (node.type == FUNC_NODE) {
      var args = [];
      for (var i = 0; i < node.arity; i++) {
        args.push(eval_stack.pop());
      }

      var result = 0.0;
      switch (node.func) {
      case ADD:
        arg0 = eval_resolve_node(args[1], inputs);
        arg1 = eval_resolve_node(args[0], inputs);
        result = arg0 + arg1;
        break;
      case SUB:
        arg0 = eval_resolve_node(args[1], inputs);
        arg1 = eval_resolve_node(args[0], inputs);
        result = arg0 - arg1;
        break;
      case MUL:
        arg0 = eval_resolve_node(args[1], inputs);
        arg1 = eval_resolve_node(args[0], inputs);
        result = arg0 * arg1;
        break;
      case DIV:
        arg0 = eval_resolve_node(args[1], inputs);
        arg1 = eval_resolve_node(args[0], inputs);
        result = arg0 / arg1;
        break;
      case POW:
        arg0 = eval_resolve_node(args[1], inputs);
        arg1 = eval_resolve_node(args[0], inputs);
        result = Math.pow(arg0, arg1);
        break;
      case EXP:
        arg0 = eval_resolve_node(args[0], inputs);
        result = Math.exp(arg0);
        break;
      case LOG:
        arg0 = eval_resolve_node(args[0], inputs);
        result = Math.log(arg0);
        break;
      }
      eval_stack.push(node_setup_const(result));

    } else {
      eval_stack.push(node);
    }
  }

  postMessage(eval_stack.pop().data);
  // console.log(eval_stack.pop());
}
