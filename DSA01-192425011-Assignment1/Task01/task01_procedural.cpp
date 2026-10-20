// Task 01 - Alternative concept: the same fare logic in a purely procedural style
// (global variables + free functions, no classes)
// Compile: g++ -std=c++17 -Wall -Wextra -o task01_procedural task01_procedural.cpp
#include <iomanip>
#include <iostream>

// Global data - any function in the program can read or change it
long long g_balance = 15000;            // Rs 150.00
long long g_basePaise[3] = {1000, 2000, 3000};
int g_peakPercent = 20;
int g_studentPercent = 25;
int g_seniorPercent = 40;
long long g_minBalance = 5000;

bool isPeak(int hour, int minute) {
    int t = hour * 60 + minute;
    return (t >= 480 && t < 600) || (t >= 1020 && t < 1200);
}

long long fare(int zone, int hour, int minute, int category) {   // 0 general, 1 student, 2 senior
    long long f = g_basePaise[zone];
    if (isPeak(hour, minute)) f = (f * (100 + g_peakPercent) + 50) / 100;
    if (category == 1) f = (f * (100 - g_studentPercent) + 50) / 100;
    if (category == 2) f = (f * (100 - g_seniorPercent) + 50) / 100;
    return f;
}

bool travel(long long f) {
    if (g_balance - f < g_minBalance) return false;
    g_balance -= f;
    return true;
}

int main() {
    int zones[] = {0, 2, 1, 2};
    int hours[] = {7, 8, 10, 19};
    int mins[]  = {45, 0, 0, 59};
    std::cout << "Procedural version, opening balance Rs " << g_balance / 100 << '\n';
    for (int i = 0; i < 4; ++i) {
        long long f = fare(zones[i], hours[i], mins[i], 1);
        bool ok = travel(f);
        std::cout << "zone " << zones[i] << "  fare " << f / 100 << '.' << std::setw(2)
                  << std::setfill('0') << f % 100 << std::setfill(' ')
                  << (ok ? "  OK" : "  DENIED") << "  balance " << g_balance / 100 << '.'
                  << std::setw(2) << std::setfill('0') << g_balance % 100 << std::setfill(' ') << '\n';
    }
    // Nothing stops any function from breaking the Rs 50 rule:
    g_balance -= 20000;
    std::cout << "After a stray 'g_balance -= 20000;' the balance is " << g_balance / 100 << " rupees\n";
}
