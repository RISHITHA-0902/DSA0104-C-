// Q2 - Return by Reference: THE DANGER (dangling reference)
// Student: N Rishitha  |  Register No: 192425011
//
// badHighest() returns a reference to a LOCAL variable. That local is
// destroyed the instant the function returns, so the returned reference
// dangles. Reading or writing through it is UNDEFINED BEHAVIOUR.
//
// Compile this on purpose to capture the compiler warning:
//     g++ -std=c++17 -Wall -Wextra q2_dangling.cpp -o q2_dangling
// Expect: warning: reference to local variable 'top' returned [-Wreturn-local-addr]

#include <iostream>

int& badHighest(int a, int b)
{
    int top = (a > b) ? a : b;   // 'top' is local to this function
    return top;                  // <-- BUG: returns reference to a dead local
}

int main()
{
    int& r = badHighest(10, 20);         // r refers to destroyed memory
    std::cout << "Value through dangling reference: " << r << '\n';
    // The printed number is not guaranteed to be 20 - it is undefined behaviour.
    return 0;
}
