#!/bin/bash
# tests/run_tests.sh
# Скрипт для запуска всех тестов

set -euo pipefail

cd "$(dirname "$0")/.."

COMPILER="./mini_compiler"
DEMO_MAIN="demo_main.c"

# Проверяем, что компилятор собран
if [[ ! -x "$COMPILER" ]]; then
    echo "Compiler not built. Run:"
    echo "  mkdir build && cd build && cmake .. && make"
    exit 1
fi

# Проверяем наличие инструментов для кросс-компиляции ARM64
if ! command -v aarch64-linux-gnu-gcc &> /dev/null; then
    echo "Warning: aarch64-linux-gnu-gcc not found. Install with:"
    echo "  sudo apt install gcc-aarch64-linux-gnu"
    echo "Skipping ARM64 tests..."
    exit 1
fi

if ! command -v qemu-aarch64 &> /dev/null; then
    echo "Warning: qemu-aarch64 not found. Install with:"
    echo "  sudo apt install qemu-user-static"
    echo "Skipping ARM64 tests..."
    exit 1
fi

# Тестовые программы и ожидаемые результаты (при arg=3)
declare -A tests=(
    ["tests/test_switch.txt"]="30"
    ["tests/test_factorial.txt"]="6"
    ["tests/test_sum_squares.txt"]="14"
)

echo "========================================"
echo "Running tests for ARM64"
echo "========================================"

for source in "${!tests[@]}"; do
    echo ""
    echo "Testing: $source"
    
    # Генерируем объектный файл
    object="${source%.txt}.o"
    "$COMPILER" "$source" --emit-obj
    
    # Линкуем с demo_main.c в статический ARM64 исполняемый файл
    exe="/tmp/$(basename "${source%.txt}").arm64"
    aarch64-linux-gnu-gcc -static "$DEMO_MAIN" "$object" -o "$exe"
    
    # Запускаем через QEMU
    output=$(qemu-aarch64 "$exe")
    
    # Извлекаем результат
    result=$(echo "$output" | grep "compiled_fn(3)" | sed 's/.*= //')
    
    if [[ "$result" == "${tests[$source]}" ]]; then
        echo "  PASS: result = $result (expected ${tests[$source]})"
    else
        echo "  FAIL: got $result, expected ${tests[$source]}"
        echo "  Full output:"
        echo "$output"
        exit 1
    fi
done

echo ""
echo "========================================"
echo "All tests passed!"
echo "========================================"