#!/bin/bash

TEST_DIR="tests"
EXECUTABLE="./itip"

GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

pass=0
fail=0

echo "Running tests..."

for test_case in "$TEST_DIR"/*; do
  if [[ -d $test_case ]]; then
    name=$(basename "$test_case")
    target_file="$test_case/target.txt"
    constraints_file="$test_case/constraints.txt"
    expected_file="$test_case/expected.txt"
    num_vars_file="$test_case/num_vars.txt"

    if [[ ! -f "$target_file" || ! -f "$constraints_file" || ! -f "$expected_file" ]]; then
      echo -e "${RED}Skipping $name: missing required files${NC}"
      continue
    fi

    num_vars=3
    if [[ -f "$num_vars_file" ]]; then
      num_vars=$(<"$num_vars_file")
    fi

    output=$($EXECUTABLE "$target_file" "$constraints_file" "$num_vars" 2>/dev/null)
    if echo "$output" | grep -q "Status: $(<"$expected_file")"; then
      echo -e "${GREEN}PASS${NC} $name"
      ((pass++))
    else
      echo -e "${RED}FAIL${NC} $name"
      echo "Output:"
      echo "$output"
      ((fail++))
    fi
  fi

done

echo
echo "Summary: $pass passed, $fail failed"
exit $fail
