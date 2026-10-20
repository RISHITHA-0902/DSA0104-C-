// Task 05 - Peak check, surcharge and concession expressions, plus the
// alternative bool peak[24] lookup table, with a timing comparison.
// Compile: g++ -std=c++17 -Wall -Wextra -O2 -o task05_expressions task05_expressions.cpp
#include <array>
#include <chrono>
#include <iostream>

constexpr int PEAK_SURCHARGE_PERCENT = 20;
constexpr int STUDENT_CONCESSION_PERCENT = 25;

// Expression version: brackets make the && / || grouping obvious
bool isPeakExpr(int hour) {
    return (hour >= 8 && hour < 10) || (hour >= 17 && hour < 20);
}

// Lookup-table version: 24 bytes, one array access per check
constexpr std::array<bool, 24> PEAK_TABLE = [] {
    std::array<bool, 24> t{};
    for (int h = 0; h < 24; ++h) t[h] = (h >= 8 && h < 10) || (h >= 17 && h < 20);
    return t;
}();
bool isPeakTable(int hour) { return PEAK_TABLE[hour]; }

int main() {
    // 1. Precedence: && binds tighter than ||, so both forms are equal for all hours
    int mismatches = 0;
    for (int hour = 0; hour < 24; ++hour) {
        bool withBrackets = (hour >= 8 && hour < 10) || (hour >= 17 && hour < 20);
        bool withoutBrackets = hour >= 8 && hour < 10 || hour >= 17 && hour < 20;  // -Wparentheses warns
        if (withBrackets != withoutBrackets || withBrackets != isPeakTable(hour)) ++mismatches;
    }
    std::cout << "Hours where the three peak checks disagree: " << mismatches << '\n';
    std::cout << "Boundary hours  8:" << isPeakExpr(8) << " 9:" << isPeakExpr(9) << " 10:" << isPeakExpr(10)
              << " 17:" << isPeakExpr(17) << " 19:" << isPeakExpr(19) << " 20:" << isPeakExpr(20) << '\n';

    // 2. Surcharge and concession with ?: and compound assignment
    long long fare = 3000;                                    // Outer, Rs 30
    int hour = 18;
    bool student = true;
    fare += isPeakExpr(hour) ? fare * PEAK_SURCHARGE_PERCENT / 100 : 0;   // ?: evaluated first, then +=
    fare -= student ? fare * STUDENT_CONCESSION_PERCENT / 100 : 0;
    std::cout << "Outer, 18:00, student fare = " << fare << " paise (expected 2700)\n";

    // 3. x++ versus ++x
    int x = 5;
    int post = x++;        // post = 5, x = 6
    int pre = ++x;         // x = 7, pre = 7
    std::cout << "post-increment gave " << post << ", pre-increment gave " << pre << ", x = " << x << '\n';

    // 4. / and % with negative operands (C++11 onwards: truncation toward zero)
    std::cout << "-7 / 2 = " << -7 / 2 << ", -7 % 2 = " << -7 % 2
              << ", 7 % -2 = " << 7 % -2 << '\n';

    // 5. Timing: expression vs table for 100 million checks
    using clock = std::chrono::steady_clock;
    const long long N = 100000000;
    volatile int sink = 0;
    auto t0 = clock::now();
    for (long long i = 0; i < N; ++i) sink = sink + isPeakExpr(static_cast<int>(i % 24));
    auto t1 = clock::now();
    for (long long i = 0; i < N; ++i) sink = sink + isPeakTable(static_cast<int>(i % 24));
    auto t2 = clock::now();
    auto ms = [](auto a, auto b) { return std::chrono::duration<double, std::milli>(b - a).count(); };
    std::cout << "100,000,000 checks: expression " << ms(t0, t1) << " ms, table " << ms(t1, t2) << " ms\n";
    std::cout << "Extra space used by the table: " << sizeof(PEAK_TABLE) << " bytes\n";
}
