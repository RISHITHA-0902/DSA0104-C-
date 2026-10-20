// Task 09 - Alternative concept: table-driven tests run in a loop.
// Adding a new boundary case = adding one line to the table.
// Compile: g++ -std=c++17 -Wall -Wextra -o task09_table_tests task09_table_tests.cpp
#include <iostream>
#include "metro_fare.hpp"

using namespace metro;

struct FareCase { const char* id; Zone zone; int hour, minute; Category cat; long long expected; };

const FareCase CASES[] = {
    {"F1", Zone::Inner, 12, 0, Category::General, 1000},
    {"F2", Zone::Inner, 9, 0, Category::General, 1200},
    {"F3", Zone::Middle, 14, 30, Category::General, 2000},
    {"F4", Zone::Middle, 18, 15, Category::General, 2400},
    {"F5", Zone::Outer, 22, 0, Category::General, 3000},
    {"F6", Zone::Outer, 8, 30, Category::General, 3600},
    {"F7", Zone::Outer, 8, 30, Category::Student, 2700},
    {"F8", Zone::Outer, 8, 30, Category::Senior, 2160},
    {"F9", Zone::Inner, 7, 59, Category::General, 1000},
    {"F10", Zone::Inner, 8, 0, Category::General, 1200},
    {"F11", Zone::Inner, 9, 59, Category::General, 1200},
    {"F12", Zone::Inner, 10, 0, Category::General, 1000},
    {"F13", Zone::Inner, 17, 0, Category::General, 1200},
    {"F14", Zone::Inner, 19, 59, Category::General, 1200},
    {"F15", Zone::Inner, 20, 0, Category::General, 1000},
    {"F16", Zone::Outer, 11, 0, Category::Student, 2250},
    {"F17", Zone::Middle, 21, 0, Category::Senior, 1200},
};

int main()
{
    int failed = 0;
    const int t = static_cast<int>(sizeof(CASES) / sizeof(CASES[0]));
    for (const FareCase& c : CASES) {
        long long actual = fare(c.zone, c.hour, c.minute, c.cat);
        bool ok = actual == c.expected;
        if (!ok) ++failed;
        std::cout << std::left << std::setw(5) << c.id << "expected " << std::setw(7) << rs(c.expected)
                  << " actual " << std::setw(7) << rs(actual) << (ok ? " PASS" : " FAIL") << '\n';
    }
    std::cout << t << " table cases, " << t - failed << " passed, " << failed << " failed\n";
    std::cout << "Table size: " << sizeof(CASES) << " bytes (" << sizeof(FareCase) << " bytes per case)\n";
    return failed ? 1 : 0;
}
