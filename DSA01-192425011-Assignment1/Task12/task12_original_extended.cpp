// Task 12 - Evaluation of the ORIGINAL design: the switch-based fare function from Task 06
// with only the enum extended by Airport. The switch is not updated, to show what breaks.
// Compile: g++ -std=c++17 -Wall -Wextra -o task12_original_extended task12_original_extended.cpp
#include <iostream>

enum class Zone { Inner, Middle, Outer, Airport };           // new value added
constexpr int BASE_PAISE[] = {1000, 2000, 3000};              // array NOT updated (size 3)
constexpr int PEAK_SURCHARGE_PERCENT = 20;

long long fare(Zone z, bool peak)
{
    long long base = 0;
    switch (z) {                                              // -Wswitch: 'Airport' not handled
        case Zone::Inner:  base = BASE_PAISE[0]; break;
        case Zone::Middle: base = BASE_PAISE[1]; break;
        case Zone::Outer:  base = BASE_PAISE[2]; break;
    }
    if (peak) base += base * PEAK_SURCHARGE_PERCENT / 100;
    return base;
}

int main()
{
    std::cout << "Airport off-peak fare from the old switch: " << fare(Zone::Airport, false)
              << " paise (should be 6000)\n";
    // A weekend discount needs a new parameter, so every call fare(z, peak) must change,
    // and 'peak' and 'weekend' could both be true - the old bool cannot express the rule
    // "the weekend discount does not combine with the peak surcharge".
    std::cout << "Outer peak (weekday) still correct: " << fare(Zone::Outer, true) << " paise\n";
}
