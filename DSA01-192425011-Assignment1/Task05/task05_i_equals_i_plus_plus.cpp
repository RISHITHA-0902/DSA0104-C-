// Task 05 - Why i = i++ is a mistake. Compile with warnings to see the diagnostic.
// Compile: g++ -std=c++17 -Wall -Wextra -o task05_i task05_i_equals_i_plus_plus.cpp
#include <iostream>

int main() {
    int i = 5;
    i = i++;   // before C++17: undefined behaviour; C++17: the increment is lost, i stays 5
    std::cout << "After i = i++ : i = " << i << " (the ++ had no lasting effect)\n";
    int j = 5;
    ++j;       // what was meant
    std::cout << "After ++j     : j = " << j << '\n';
}
