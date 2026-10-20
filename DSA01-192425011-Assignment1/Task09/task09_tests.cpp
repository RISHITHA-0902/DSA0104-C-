// Task 09 - Implement and test the fare logic with separate hand-written checks
// Compile: g++ -std=c++17 -Wall -Wextra -o task09_tests task09_tests.cpp
#include <iostream>
#include "metro_fare.hpp"

using namespace metro;

int passed = 0, failed = 0;

void check(const std::string& id, const std::string& what, const std::string& expected, const std::string& actual)
{
    const bool ok = expected == actual;
    ok ? ++passed : ++failed;
    std::cout << std::left << std::setw(5) << id << std::setw(46) << what << std::setw(12) << expected
              << std::setw(12) << actual << (ok ? "PASS" : "FAIL") << '\n';
}

std::string yesNo(bool b) { return b ? "yes" : "no"; }

int main()
{
    std::cout << std::left << std::setw(5) << "ID" << std::setw(46) << "Test case" << std::setw(12) << "Expected"
              << std::setw(12) << "Actual" << "Result\n" << std::string(80, '-') << '\n';

    // ---- Normal journeys: each zone, off-peak and peak (general passenger) ----
    check("N1", "Inner  off-peak 12:00", "10.00", rs(fare(Zone::Inner, 12, 0, Category::General)));
    check("N2", "Inner  peak     09:00", "12.00", rs(fare(Zone::Inner, 9, 0, Category::General)));
    check("N3", "Middle off-peak 14:30", "20.00", rs(fare(Zone::Middle, 14, 30, Category::General)));
    check("N4", "Middle peak     18:15", "24.00", rs(fare(Zone::Middle, 18, 15, Category::General)));
    check("N5", "Outer  off-peak 22:00", "30.00", rs(fare(Zone::Outer, 22, 0, Category::General)));
    check("N6", "Outer  peak     08:30", "36.00", rs(fare(Zone::Outer, 8, 30, Category::General)));
    check("N7", "Outer  peak student (36 x 0.75)", "27.00", rs(fare(Zone::Outer, 8, 30, Category::Student)));
    check("N8", "Outer  peak senior  (36 x 0.60)", "21.60", rs(fare(Zone::Outer, 8, 30, Category::Senior)));

    // ---- Boundary cases ----
    check("B1", "07:59 is off-peak", "no", yesNo(isPeak(7, 59)));
    check("B2", "08:00 is peak", "yes", yesNo(isPeak(8, 0)));
    check("B3", "09:59 is peak", "yes", yesNo(isPeak(9, 59)));
    check("B4", "10:00 is off-peak", "no", yesNo(isPeak(10, 0)));
    check("B5", "17:00 is peak", "yes", yesNo(isPeak(17, 0)));
    check("B6", "19:59 is peak", "yes", yesNo(isPeak(19, 59)));
    check("B7", "20:00 is off-peak", "no", yesNo(isPeak(20, 0)));
    {
        SmartCard c{8000};   // Rs 80, fare Rs 30 leaves exactly Rs 50
        bool ok = c.deduct(3000);
        check("B8", "Rs 80 - Rs 30 leaves exactly Rs 50: allowed?", "yes", yesNo(ok));
        check("B9", "Balance after B8", "50.00", rs(c.balance()));
    }
    {
        SmartCard c{7999};   // leaves Rs 49.99
        check("B10", "Rs 79.99 - Rs 30 leaves Rs 49.99: allowed?", "no", yesNo(c.deduct(3000)));
        check("B11", "Balance unchanged after denial", "79.99", rs(c.balance()));
    }

    // ---- Invalid cases ----
    check("I1", "Unknown zone number 4 rejected", "rejected", zoneFromNumber(4) ? "accepted" : "rejected");
    check("I2", "Zone number 0 rejected", "rejected", zoneFromNumber(0) ? "accepted" : "rejected");
    {
        SmartCard c{10000};
        check("I3", "Negative recharge -500 rejected", "rejected", c.recharge(-500) ? "accepted" : "rejected");
        check("I4", "Balance unchanged after I3", "100.00", rs(c.balance()));
        check("I5", "Negative fare cannot be deducted", "rejected", c.deduct(-100) ? "accepted" : "rejected");
    }
    check("I6", "Hour typed as \"ab\" rejected", "rejected", parseHour("ab") ? "accepted" : "rejected");
    check("I7", "Hour typed as \"9x\" rejected", "rejected", parseHour("9x") ? "accepted" : "rejected");
    check("I8", "Hour typed as \"24\" rejected", "rejected", parseHour("24") ? "accepted" : "rejected");
    check("I9", "Hour typed as \"17\" accepted", "17", std::to_string(parseHour("17").value_or(-1)));

    // ---- 5 journeys with a hand-calculated total ----
    // 1 Inner  off-peak general  10.00
    // 2 Middle peak     general  24.00
    // 3 Outer  off-peak student  22.50  (30 x 0.75)
    // 4 Outer  peak     senior   21.60  (36 x 0.60)
    // 5 Middle off-peak senior   12.00  (20 x 0.60)
    // Total = 90.10 ; Rs 200.00 - 90.10 = Rs 109.90
    struct J { Zone z; int h, m; Category c; };
    const J five[] = {{Zone::Inner, 13, 0, Category::General}, {Zone::Middle, 9, 15, Category::General},
                      {Zone::Outer, 11, 0, Category::Student}, {Zone::Outer, 18, 0, Category::Senior},
                      {Zone::Middle, 21, 0, Category::Senior}};
    SmartCard card{20000};
    long long total = 0;
    for (const J& j : five) {
        long long f = fare(j.z, j.h, j.m, j.c);
        if (card.deduct(f)) total += f;
    }
    check("T1", "Total of 5 journeys (hand: 90.10)", "90.10", rs(total));
    check("T2", "Balance after 5 journeys (hand: 109.90)", "109.90", rs(card.balance()));

    std::cout << std::string(80, '-') << "\nPassed " << passed << ", failed " << failed << '\n';
    return failed == 0 ? 0 : 1;
}
