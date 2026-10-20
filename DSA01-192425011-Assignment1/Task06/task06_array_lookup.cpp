// Task 06 - Alternative concept: base fare from an array indexed by the zone
// instead of a switch. Timing comparison of both lookups.
// Compile: g++ -std=c++17 -Wall -Wextra -O2 -o task06_array_lookup task06_array_lookup.cpp
#include <chrono>
#include <iostream>

enum class Zone { Inner, Middle, Outer };
constexpr long long FARE[] = {1000, 2000, 3000};

long long fareSwitch(Zone z)
{
    switch (z) {
        case Zone::Inner:  return 1000;
        case Zone::Middle: return 2000;
        case Zone::Outer:  return 3000;
    }
    return -1;                                   // unreachable for valid zones
}

long long fareArray(Zone z) { return FARE[static_cast<int>(z)]; }

int main()
{
    for (int i = 0; i < 3; ++i)
        std::cout << "zone " << i << ": switch " << fareSwitch(static_cast<Zone>(i))
                  << "  array " << fareArray(static_cast<Zone>(i)) << '\n';

    using clock = std::chrono::steady_clock;
    const long long N = 100000000;
    volatile long long sink = 0;
    auto t0 = clock::now();
    for (long long i = 0; i < N; ++i) sink = sink + fareSwitch(static_cast<Zone>(i % 3));
    auto t1 = clock::now();
    for (long long i = 0; i < N; ++i) sink = sink + fareArray(static_cast<Zone>(i % 3));
    auto t2 = clock::now();
    auto ms = [](auto a, auto b) { return std::chrono::duration<double, std::milli>(b - a).count(); };
    std::cout << "100,000,000 lookups: switch " << ms(t0, t1) << " ms, array " << ms(t1, t2) << " ms\n";
    std::cout << "Array size: " << sizeof(FARE) << " bytes (" << sizeof(FARE) / sizeof(FARE[0]) << " zones)\n";
}
