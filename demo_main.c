// demo_main.c
// Тестовая программа для линковки с сгенерированным объектным файлом
// Демонстрирует вызов функции compiled_fn из вашего компилятора

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

// Функция, генерируемая вашим компилятором
// Сигнатура: int64_t compiled_fn(int64_t arg)
extern int64_t compiled_fn(int64_t arg);

int main(void)
{
    // Тестовые значения
    int64_t test_values[] = {1, 2, 3, 5, 10, 42};
    size_t num_tests = sizeof(test_values) / sizeof(test_values[0]);

    printf("========================================\n");
    printf("Testing compiled_fn from mini-compiler\n");
    printf("========================================\n\n");

    for (size_t i = 0; i < num_tests; ++i)
    {
        int64_t input = test_values[i];
        int64_t result = compiled_fn(input);
        printf("compiled_fn(%" PRId64 ") = %" PRId64 "\n", input, result);
    }

    printf("\n========================================\n");
    printf("All tests completed.\n");

    return 0;
}