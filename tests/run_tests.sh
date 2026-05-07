#!/bin/bash

cd "$(dirname "$0")/.."

COMPILER="./mini_compiler"

declare -A tests=(
    ["tests/test_switch.txt"]="30"
    ["tests/test_factorial.txt"]="6"
    ["tests/test_sum_squares.txt"]="14"
)

echo "========================================"
echo "Running tests"
echo "========================================"

for source in "${!tests[@]}"; do
    echo ""
    echo "Testing: $(basename $source)"
    
    "$COMPILER" "$source" --emit-obj
    
    aarch64-linux-gnu-gcc -static demo_main.c "${source%.txt}.o" -o /tmp/test_app
    
    result=$(qemu-aarch64 /tmp/test_app | grep "compiled_fn(3)" | sed 's/.*= //')
    
    if [[ "$result" == "${tests[$source]}" ]]; then
        echo "  PASS (result = $result)"
    else
        echo "  FAIL (expected ${tests[$source]}, got $result)"
        exit 1
    fi
done

echo ""
echo "========================================"
echo "All tests passed!"
echo "========================================"