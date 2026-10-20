// Task 04 - Evidence: brace initialisation refuses narrowing conversions.
// This file is EXPECTED TO FAIL to compile. The error message is saved in output/.
// Compile: g++ -std=c++17 -Wall -Wextra -c task04_narrowing_error.cpp
int main()
{
    int fareA = 3.7;          // copy initialisation: silently becomes 3
    int fareB{3.7};           // brace initialisation: compile-time ERROR (narrowing)
    long long paise = 3600;
    int small{paise};         // ill-formed narrowing: GCC reports -Wnarrowing (an error with -Werror)
    return fareA + fareB + small;
}
