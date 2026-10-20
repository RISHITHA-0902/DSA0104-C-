// Task 10 - A deliberately careless version of the gate code, used to collect the
// warnings produced by -Wall -Wextra. The log is saved in output/compile_warnings.txt.
// Compile: g++ -std=c++17 -Wall -Wextra -o task10_warnings_demo task10_warnings_demo.cpp
#include <iostream>
#include <vector>

enum class Zone { Inner, Middle, Outer };

int baseFare(Zone z)
{
    switch (z) {                       // -Wswitch: Outer not handled
        case Zone::Inner:  return 1000;
        case Zone::Middle: return 2000;
    }
    return 0;
}

bool isPeak(int hour, int minute)      // -Wunused-parameter: minute
{
    return hour >= 8 && hour < 10 || hour >= 17 && hour < 20;   // -Wparentheses
}

int main()
{
    std::vector<int> fares = {1000, 2000, 3000};
    int unusedTotal = 0;               // -Wunused-variable
    for (int i = 0; i < fares.size(); ++i)   // -Wsign-compare
        std::cout << fares[i] << ' ';
    double rupees = 21.6;
    int paise = rupees * 100;          // silent narrowing (no warning without -Wconversion)
    std::cout << '\n' << baseFare(Zone::Outer) << ' ' << isPeak(10, 0) << ' ' << paise << '\n';
}
