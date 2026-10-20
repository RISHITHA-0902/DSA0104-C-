// Task 02 - Tokens, keywords, identifiers and constants
// All fare values are defined ONCE, as typed constexpr constants and an enum class.
// Compile: g++ -std=c++17 -Wall -Wextra -o task02_constants task02_constants.cpp
#include <iostream>

namespace fare {                                   // keyword 'namespace', identifier 'fare'
    enum class Zone : int { Inner, Middle, Outer, Count };

    constexpr long long BASE_PAISE[static_cast<int>(Zone::Count)] = {1000, 2000, 3000};
    constexpr int PEAK_SURCHARGE_PERCENT = 20;
    constexpr int STUDENT_CONCESSION_PERCENT = 25;
    constexpr int SENIOR_CONCESSION_PERCENT = 40;
    constexpr long long MIN_BALANCE_PAISE = 5000;   // Rs 50
    constexpr int MORNING_PEAK_START = 8 * 60, MORNING_PEAK_END = 10 * 60;   // minutes
    constexpr int EVENING_PEAK_START = 17 * 60, EVENING_PEAK_END = 20 * 60;

    // constexpr function: evaluated by the compiler when the arguments are constants
    constexpr long long peakFare(Zone z) {
        return BASE_PAISE[static_cast<int>(z)] * (100 + PEAK_SURCHARGE_PERCENT) / 100;
    }
}

// Plain (unscoped) enum - shown only to compare with enum class
enum OldZone { INNER, MIDDLE, OUTER };
enum Colour { RED, GREEN, BLUE };

int main() {
    using fare::Zone;

    // Compile-time checks: if a revision breaks a rule the program does not compile
    static_assert(fare::peakFare(Zone::Outer) == 3600, "Outer peak fare must be Rs 36");
    static_assert(fare::MIN_BALANCE_PAISE > 0, "Minimum balance must be positive");

    constexpr long long outerPeak = fare::peakFare(Zone::Outer);   // computed at compile time
    std::cout << "Outer peak fare (constexpr, compile time): " << outerPeak << " paise\n";

    // ---- enum vs enum class ----
    OldZone oz = OUTER;
    int asInt = oz;                       // unscoped enum converts silently to int
    bool strange = (OUTER == BLUE);       // compiles: compares two unrelated enums as ints
    std::cout << "Plain enum OUTER as int = " << asInt
              << ", OUTER == BLUE ? " << std::boolalpha << strange << '\n';

    Zone z = Zone::Outer;
    // int bad = z;                        // ERROR: no implicit conversion from enum class
    // if (z == BLUE) {}                   // ERROR: cannot compare Zone with Colour
    std::cout << "enum class Zone::Outer needs static_cast: " << static_cast<int>(z) << '\n';

    // ---- the five kinds of tokens in one statement ----
    //   long long fareNow = fare::BASE_PAISE[2] * 120 / 100;
    //   keywords    : long, long
    //   identifiers : fareNow, fare, BASE_PAISE
    //   constants   : 2, 120, 100
    //   operators   : =, ::, [], *, /
    //   punctuators : ; [ ]
    long long fareNow = fare::BASE_PAISE[2] * 120 / 100;
    std::cout << "fareNow = " << fareNow << " paise\n";

    std::cout << "sizeof(fare::BASE_PAISE) = " << sizeof(fare::BASE_PAISE) << " bytes\n";
}
