// Task 04 - Corrected version: every conversion is explicit (static_cast) and
// brace initialisation is used everywhere, so narrowing is caught at compile time.
// Compile: g++ -std=c++17 -Wall -Wextra -o task04_explicit task04_explicit.cpp
#include <iostream>

int main()
{
    int a{7}, b{2};
    double c{static_cast<double>(a / b)};           // integer division is now visible: 3
    double d{static_cast<double>(a) / b};           // real division: 3.5
    std::cout << c << ' ' << d << '\n';

    char ch{static_cast<char>('A' + 2)};            // 'C'
    std::cout << ch << ' ' << static_cast<char>(ch + 1) << '\n';   // prints C D

    bool flag{5 != 0};                              // the intention is written down
    std::cout << flag << ' ' << std::boolalpha << flag << '\n';

    unsigned int u{1};
    int s{-1};
    // compare in a type that can hold both values
    std::cout << (static_cast<long long>(s) < static_cast<long long>(u) ? "s < u" : "s >= u") << '\n';
}
