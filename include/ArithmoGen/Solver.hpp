#pragma once

#include "Expression.hpp"
#include <mutex>
#include <string>
#include <vector>

namespace ArithmoGen {

struct SolverOptions {
  bool ordered = true;
  bool allow_unary = false;
  bool allow_concat = false;
  bool find_all = false;
};

class Solver {
public:
  Solver(std::vector<int> numbers, double target);

  // Main solve method
  void solve(const SolverOptions &options);

private:
  std::vector<int> numbers_;
  double target_;

  // Determine if we found a solution
  bool solution_found_ = false;

  std::mutex output_mutex_;

  void solve_impl(std::vector<int> nums, const SolverOptions &options);
  void process_permutation(const std::vector<int> &p_nums,
                           const SolverOptions &options);
  void generate_expressions(std::vector<Expression> &exprs,
                            const SolverOptions &options);
};

} // namespace ArithmoGen
