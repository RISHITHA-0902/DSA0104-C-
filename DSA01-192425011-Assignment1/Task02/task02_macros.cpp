// Task 02 - Alternative concept: the same constants written with #define macros
// Shows why macros are unsafe: no type, no scope, text replacement only.
// Compile: g++ -std=c++17 -Wall -Wextra -o task02_macros task02_macros.cpp
#include <iostream>

#define INNER_FARE 1000
#define MIDDLE_FARE 2000
#define OUTER_FARE 3000
#define PEAK_FACTOR 100 + 20          // forgot brackets - a classic macro bug
#define MIN_BALANCE 5000
#define SQUARE(x) x * x               // function-like macro without brackets

int main() {
    // Expected: 3000 * 120 / 100 = 3600
    // Macro expands to: 3000 * 100 + 20 / 100 = 300000 + 0 = 300000
    long long wrong = OUTER_FARE * PEAK_FACTOR / 100;
    std::cout << "Outer peak fare with macro  : " << wrong << " paise (expected 3600)\n";

    // Expected SQUARE(1+1) = 4, macro gives 1+1*1+1 = 3
    std::cout << "SQUARE(1 + 1) with macro    : " << SQUARE(1 + 1) << " (expected 4)\n";

    // A macro has no scope: this local variable name would be replaced by the preprocessor.
    // int MIN_BALANCE = 10;   // becomes 'int 5000 = 10;' -> confusing compile error

    // A macro has no type: the compiler sees only the number 5000 (an int literal)
    std::cout << "MIN_BALANCE has no name in the debugger, only the value " << MIN_BALANCE << '\n';
    std::cout << "Correct value with brackets : " << OUTER_FARE * (PEAK_FACTOR) / 100 << " paise\n";
}
