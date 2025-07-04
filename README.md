# Minimalist Shannon Prover

A lightweight C++ tool to verify Shannon-type information inequalities using linear programming. Inspired by ITIP, built using GLPK.

---

## Features
- Checks validity of target inequalities like `I(X;Z) >= I(X;Y)` under Shannon-type constraints
- Accepts user-specified equality and inequality constraints (e.g. `I(X;Z|Y) = 0`, `H(X) <= 1`)
- Uses GLPK backend to solve LPs
- Command line interface
- Automated test suite

---

## Requirements
- C++17 or later
- [GLPK](https://www.gnu.org/software/glpk/)

To install GLPK on Ubuntu:
```bash
sudo apt install libglpk-dev
```

---

## Build
```bash
make
```
This compiles the tool into the `itip` binary.

---

## Usage
```bash
./itip <target_file> <constraint_file> [num_variables]
```

- `target_file`: contains a single inequality, e.g. `I(A;B) - I(A;C) >= 0`
- `constraint_file`: contains one constraint per line, e.g.
  ```
  I(A;C|B) = 0
  ```
- `num_variables`: optional (default = 3). Variables used are A, B, C, ..., X26, X27, etc.

---

## Example (Data Processing Inequality)
### `target.txt`
```
I(A;B) - I(A;C) >= 0
```
### `constraints.txt`
```
I(A;C|B) = 0
```
### Run
```bash
./itip target.txt constraints.txt 3
```
Output:
```
Target inequality:
+1*H(A) +1*H(B) +1*H(C) -1*H(A,C) >= 0

Status: VALID
```

---

## Testing
A testing script `run_tests` is included.

To run all tests:
```bash
chmod +x run_tests.sh
./run_tests.sh
```
Each test lives in a folder under `tests/`, with:
```
target.txt         # target inequality
constraints.txt    # user constraints
expected.txt       # expected result (VALID or NOT VALID)
num_vars.txt       # optional
```

---

## Directory Structure
```
include/                # header files
src/                    # source files
main.cpp                # standalone entry point
cli.cpp                 # CLI main
Makefile
README.md
run_tests               # test runner
```

---

## Acknowledgments
- "A Framework for Linear Information Inequalities" Raymond W. Yeung
- GLPK for LP solving

---

Feel free to suggest features or open issues!
