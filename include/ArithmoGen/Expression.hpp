#pragma once

#include <cmath>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace ArithmoGen {

enum class Operator {
  ADD,
  SUB,
  MUL,
  DIV,
  NONE // Represents a leaf node (number)
};

inline std::string op_to_string(Operator op) {
  switch (op) {
  case Operator::ADD:
    return "+";
  case Operator::SUB:
    return "-";
  case Operator::MUL:
    return "*";
  case Operator::DIV:
    return "/";
  default:
    return "";
  }
}

// Represents a node in the expression tree or a computed value
struct Expression {
  double value;
  std::string representation; // String representation like "(1 + 2)"
  int priority =
      100; // For parenthesis optimization: 0 for +-, 1 for */, 100 for leaf

  Expression() : value(0), priority(100) {}

  Expression(double v, const std::string &s, int p = 100)
      : value(v), representation(s), priority(p) {}

  // Leaf constructor
  Expression(double v) : value(v), priority(100) {
    // Simple heuristic for clean integer printing
    if (std::abs(v - std::round(v)) < 1e-9) {
      representation = std::to_string(static_cast<long long>(std::round(v)));
    } else {
      representation = std::to_string(v);
      // Remove trailing zeros
      size_t last = representation.find_last_not_of('0');
      if (last != std::string::npos) {
        representation.erase(last + 1, std::string::npos);
        if (representation.back() == '.')
          representation.pop_back();
      }
    }
  }

  // Operation constructor
  Expression(const Expression &left, const Expression &right, Operator op)
      : priority(0) {
    double val = 0;
    char op_char = '?';
    int op_priority = 0;
    switch (op) {
    case Operator::ADD:
      val = left.value + right.value;
      op_char = '+';
      op_priority = 0;
      break;
    case Operator::SUB:
      val = left.value - right.value;
      op_char = '-';
      op_priority = 0;
      break;
    case Operator::MUL:
      val = left.value * right.value;
      op_char = '*';
      op_priority = 1;
      break;
    case Operator::DIV:
      if (std::abs(right.value) > 1e-9)
        val = left.value / right.value;
      else
        val = NAN;
      op_char = '/';
      op_priority = 1;
      break;
    default:
      break;
    }
    value = val;
    priority = op_priority;

    // Parenthesis logic
    std::string l_str = left.representation;
    if (left.priority < op_priority)
      l_str = "(" + l_str + ")";

    std::string r_str = right.representation;
    // Right side usually needs parens if same priority for non-associative (-,
    // /) or lower priority.
    if (right.priority < op_priority ||
        (right.priority == op_priority &&
         (op == Operator::SUB || op == Operator::DIV))) {
      r_str = "(" + r_str + ")";
    }

    representation = l_str + " " + op_char + " " + r_str;
  }
};

} // namespace ArithmoGen
