#include <algorithm>
#include <chrono>
#include <iostream>
#include <string>
#include <vector>

#include "ArithmoGen/Solver.hpp"

void print_usage() {
  std::cout
      << "Usage: ArithmoGen --numbers <n1 n2 ...> --target <t> [options]\n"
      << "Options:\n"
      << "  -n, --numbers <list>   List of numbers (space separated)\n"
      << "  -t, --target <val>     Target number\n"
      << "  -o, --ordered          Preserve order (default: true). Use "
         "--no-ordered to shuffle.\n"
      << "  -u, --unary            Allow unary minus\n"
      << "  -c, --concat           Allow concatenation\n"
      << "  -a, --all              Find all solutions\n"
      << "  -h, --help             Show this help\n";
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    print_usage();
    return 1;
  }

  std::vector<int> numbers;
  double target = 0;
  bool target_set = false;
  ArithmoGen::SolverOptions options;

  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg == "--numbers" || arg == "-n") {
      while (i + 1 < argc) {
        std::string next_arg = argv[i + 1];
        if (next_arg[0] == '-') {
          if (next_arg.length() > 1 && isdigit(next_arg[1])) {
            // Negative number
          } else {
            // Likely a flag
            break;
          }
        }

        // Heuristic: if '-' follows by digit, it's a number.
        // Or if it's just a number string.
        if (next_arg.find("--") == 0)
          break;

        try {
          numbers.push_back(std::stoi(next_arg));
        } catch (...) {
          break;
        }
        i++;
      }
    } else if (arg == "--target" || arg == "-t") {
      if (i + 1 < argc) {
        try {
          target = std::stod(argv[++i]);
          target_set = true;
        } catch (...) {
        }
      }
    } else if (arg == "--ordered" || arg == "-o") {
      options.ordered = true;
    } else if (arg == "--no-ordered") {
      options.ordered = false;
    } else if (arg == "--unary" || arg == "-u") {
      options.allow_unary = true;
    } else if (arg == "--concat" || arg == "-c") {
      options.allow_concat = true;
    } else if (arg == "--all" || arg == "-a") {
      options.find_all = true;
    } else if (arg == "--help" || arg == "-h") {
      print_usage();
      return 0;
    }
  }

  if (numbers.empty() || !target_set) {
    std::cerr << "Error: Numbers and target must be specified.\n";
    return 1;
  }

  std::cout << "ArithmoGen v1.0 | Target: " << target << " | Numbers: ";
  for (int n : numbers)
    std::cout << n << " ";
  std::cout << "\n";
  std::cout << "Configuration: " << (options.ordered ? "ORDERED" : "UNORDERED")
            << (options.allow_concat ? ", CONCAT" : "")
            << (options.allow_unary ? ", UNARY" : "") << "\n";

  auto start = std::chrono::high_resolution_clock::now();

  ArithmoGen::Solver solver(numbers, target);
  solver.solve(options);

  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> elapsed = end - start;
  std::cout << "Execution Time: " << elapsed.count() << " ms\n";

  return 0;
}
