# ArithmoGen

**Project:** Arithmetic Expression Generator & Solver (C++ Implementation)
**Author:** Semih Taha AKSOY (WlayerX)
**Version:** 1.0.0
**License:** MIT License

## Overview

ArithmoGen is a command-line utility designed to solve arithmetic puzzles by generating expression trees from a given set of integer inputs. The application utilizes a recursive backtracking algorithm optimized for performance, capable of handling standard arithmetic operations (+, -, *, /) as well as investigating permutations, concatenation, and unary negation options to reach a specified target value.

This project demonstrates modern C++17 standards, focusing on efficient memory management, thread-safe parallel processing for permutation sets, and strict type safety.

## Technical Specifications

- **Language Standard:** C++17
- **Build System:** CMake (Version 3.10+)
- **Concurrency:** `std::thread`, `std::async`, `std::mutex` for parallelized search of unordered sets.
- **Precision:** Double-precision floating-point arithmetic with epsilon comparisons for equality checks.

## Compilation and Installation

The project uses CMake for build configuration. Ensure a compatible C++ compiler (GCC, Clang, or MSVC) is installed.

### Build Steps

1.  Navigate to the project root directory.
2.  Create a build directory:
    ```bash
    mkdir build
    cd build
    ```
3.  Configure the project:
    ```bash
    cmake ..
    ```
4.  Compile the source code:
    ```bash
    make
    ```

The executable `ArithmoGen` will be generated in the build directory.

## Usage Guide

The tool accepts command-line arguments to define the input set, target value, and operational constraints.

### Syntax

```bash
./ArithmoGen --numbers <n1 n2 ...> --target <value> [options]
```

### Arguments

-   `--numbers, -n`: Space-separated list of input integers.
-   `--target, -t`: The target integer or floating-point value to compute.
-   `--ordered, -o`: Restrict search to the given order of numbers (default behavior).
-   `--no-ordered`: Enable permutation generation to search all possible orderings.
-   `--concat, -c`: Allow concatenation of adjacent digits (e.g., 1 and 2 becoming 12).
-   `--unary, -u`: Permit unary negation of input numbers.
-   `--all, -a`: Continue search after finding the first valid solution to list all possibilities.

### Execution Examples

**Standard Search (Ordered):**
```bash
./ArithmoGen --numbers 1 2 3 4 --target 10 --ordered
```

**Permutation Search (Unordered):**
```bash
./ArithmoGen --numbers 1 5 7 2 --target 29 --no-ordered
```

**Complex Operations (Concatenation & Unary):**
```bash
./ArithmoGen --numbers 1 2 3 4 --target 120 --concat --unary
```

## Performance Notes

For small sets (N < 7), the solver operates nearly instantaneously. As N increases, the search space grows factorially. This implementation mitigates execution time by parallelizing the search across available CPU cores when the `--no-ordered` flag is active.

## Disclaimer

This software is provided "as is", without warranty of any kind. It is intended for educational and algorithmic demonstration purposes.
