#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

extern int64_t compiled_fn(int64_t arg);

int main(void) {
    int64_t test_values[] = {1, 2, 3, 4, 5, 10};
    size_t num_tests = sizeof(test_values) / sizeof(test_values[0]);
    
    printf("========================================\n");
    printf("Testing compiled_fn from mini-compiler\n");
    printf("========================================\n\n");
    
    for (size_t i = 0; i < num_tests; ++i) {
        int64_t input = test_values[i];
        int64_t result = compiled_fn(input);
        printf("compiled_fn(%" PRId64 ") = %" PRId64 "\n", input, result);
    }
    
    printf("\n========================================\n");
    printf("All tests completed.\n");
    
    return 0;
}