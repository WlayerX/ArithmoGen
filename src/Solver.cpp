#include "ArithmoGen/Solver.hpp"
#include <algorithm>
#include <cmath>
#include <future>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

namespace ArithmoGen {

Solver::Solver(std::vector<int> numbers, double target)
    : numbers_(std::move(numbers)), target_(target) {}

// Helper to concat numbers
static std::vector<std::vector<int>>
apply_concat(const std::vector<int> &nums) {
  std::vector<std::vector<int>> results;
  int n = nums.size();
  if (n == 0)
    return results;
  if (n == 1) {
    results.push_back(nums);
    return results;
  }

  int gaps = n - 1;
  unsigned long long total_patterns = 1ULL << gaps;

  for (unsigned long long mask = 0; mask < total_patterns; ++mask) {
    std::vector<int> current_set;
    std::string current_num_str = std::to_string(nums[0]);

    for (int i = 0; i < gaps; ++i) {
      if ((mask >> i) & 1) {
        // Join
        current_num_str += std::to_string(nums[i + 1]);
      } else {
        // Break
        current_set.push_back(std::stoi(current_num_str));
        current_num_str = std::to_string(nums[i + 1]);
      }
    }
    current_set.push_back(std::stoi(current_num_str));
    results.push_back(current_set);
  }
  return results;
}

void Solver::solve(const SolverOptions &options) {
  if (options.ordered) {
    solve_impl(numbers_, options);
  } else {
    std::vector<int> sorted_nums = numbers_;
    std::sort(sorted_nums.begin(), sorted_nums.end());

    // Get unique starting numbers
    std::vector<int> unique_starts;
    if (!sorted_nums.empty()) {
      unique_starts.push_back(sorted_nums[0]);
      for (size_t i = 1; i < sorted_nums.size(); ++i) {
        if (sorted_nums[i] != sorted_nums[i - 1]) {
          unique_starts.push_back(sorted_nums[i]);
        }
      }
    }

    std::vector<std::future<void>> futures;

    for (int start_val : unique_starts) {
      std::vector<int> remaining_nums = sorted_nums;
      auto it =
          std::find(remaining_nums.begin(), remaining_nums.end(), start_val);
      if (it != remaining_nums.end())
        remaining_nums.erase(it);

      futures.push_back(std::async(
          std::launch::async, [this, start_val, remaining_nums, options]() {
            std::vector<int> current_nums;
            current_nums.push_back(start_val);
            current_nums.insert(current_nums.end(), remaining_nums.begin(),
                                remaining_nums.end());

            std::sort(current_nums.begin() + 1, current_nums.end());

            do {
              if (this->solution_found_ && !options.find_all)
                return;
              process_permutation(current_nums, options);
            } while (std::next_permutation(current_nums.begin() + 1,
                                           current_nums.end()));
          }));
    }

    for (auto &f : futures) {
      f.wait();
    }
  }

  if (!solution_found_) {
    std::lock_guard<std::mutex> lock(output_mutex_);
    std::cout << "No solution found.\n";
  }
}

void Solver::solve_impl(std::vector<int> nums, const SolverOptions &options) {
  if (options.ordered) {
    process_permutation(nums, options);
  } else {
    std::sort(nums.begin(), nums.end());
    do {
      if (solution_found_ && !options.find_all)
        return;
      process_permutation(nums, options);
    } while (std::next_permutation(nums.begin(), nums.end()));
  }
}

void Solver::process_permutation(const std::vector<int> &p_nums,
                                 const SolverOptions &options) {
  std::vector<std::vector<int>> number_groups;
  if (options.allow_concat) {
    number_groups = apply_concat(p_nums);
  } else {
    number_groups.push_back(p_nums);
  }

  for (const auto &group : number_groups) {
    int u_n = group.size();
    long long unary_combinations = options.allow_unary ? (1LL << u_n) : 1;

    for (long long u_mask = 0; u_mask < unary_combinations; ++u_mask) {
      std::vector<Expression> initial_exprs;
      initial_exprs.reserve(u_n);
      for (int i = 0; i < u_n; ++i) {
        double val = static_cast<double>(group[i]);
        if ((u_mask >> i) & 1)
          val = -val;
        initial_exprs.emplace_back(val);
      }

      generate_expressions(initial_exprs, options);
      if (solution_found_ && !options.find_all)
        return;
    }
  }
}

void Solver::generate_expressions(std::vector<Expression> &exprs,
                                  const SolverOptions &options) {
  if (exprs.size() == 1) {
    if (std::abs(exprs[0].value - target_) < 1e-9) {
      std::lock_guard<std::mutex> lock(output_mutex_);
      if (!solution_found_ || options.find_all) {
        std::cout << "[Solution] " << exprs[0].representation << " = "
                  << target_ << "\n";
        solution_found_ = true;
      }
    }
    return;
  }

  if (solution_found_ && !options.find_all)
    return;

  for (size_t i = 0; i < exprs.size() - 1; ++i) {
    Expression &left = exprs[i];
    Expression &right = exprs[i + 1];

    static const std::vector<Operator> ops = {Operator::ADD, Operator::SUB,
                                              Operator::MUL, Operator::DIV};

    for (auto op : ops) {
      if (op == Operator::DIV && std::abs(right.value) < 1e-9)
        continue;

      std::vector<Expression> next_exprs;
      next_exprs.reserve(exprs.size() - 1);

      for (size_t k = 0; k < i; ++k)
        next_exprs.push_back(exprs[k]);
      next_exprs.emplace_back(left, right, op);
      for (size_t k = i + 2; k < exprs.size(); ++k)
        next_exprs.push_back(exprs[k]);

      generate_expressions(next_exprs, options);
      if (solution_found_ && !options.find_all)
        return;
    }
  }
}

} // namespace ArithmoGen
