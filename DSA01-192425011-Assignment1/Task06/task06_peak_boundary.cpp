// Task 06 - Extension: journeys at exactly 08:00, 10:00, 17:00 and 20:00.
// Peak is decided from the clock time with the rule [08:00,10:00) and [17:00,20:00).
// Also shows break / continue for invalid journeys and a while loop over a day's input.
// Compile: g++ -std=c++17 -Wall -Wextra -o task06_peak_boundary task06_peak_boundary.cpp
#include <iomanip>
#include <iostream>

enum class Zone { Inner, Middle, Outer };
constexpr int BASE_PAISE[] = {1000, 2000, 3000};
constexpr int PEAK_SURCHARGE_PERCENT = 20;
constexpr long long MIN_BALANCE_PAISE = 5000;

bool isPeak(int hour, int minute)
{
    const int t = hour * 60 + minute;
    return (t >= 8 * 60 && t < 10 * 60) || (t >= 17 * 60 && t < 20 * 60);
}

long long fare(Zone z, bool peak)
{
    long long base = 0;
    switch (z) {
        case Zone::Inner:  base = BASE_PAISE[0]; break;
        case Zone::Middle: base = BASE_PAISE[1]; break;
        case Zone::Outer:  base = BASE_PAISE[2]; break;
    }
    if (peak) base += base * PEAK_SURCHARGE_PERCENT / 100;
    return base;
}

void printRs(long long p) { std::cout << p / 100 << '.' << std::setw(2) << std::setfill('0') << p % 100 << std::setfill(' '); }

int main()
{
    struct Trip { int zone; int hour; int minute; };
    const Trip trips[] = {
        {0, 7, 59}, {0, 8, 0}, {1, 9, 59}, {1, 10, 0},     // morning boundaries
        {2, 16, 59}, {2, 17, 0}, {2, 19, 59}, {2, 20, 0},  // evening boundaries
        {5, 9, 0},                                         // invalid zone -> continue
        {0, 25, 0},                                        // invalid hour -> continue
    };
    long long balance = 50000;                             // Rs 500
    std::cout << "Zone   Time   Peak  Fare(Rs)  Balance(Rs)\n";
    for (const Trip& t : trips) {
        if (t.zone < 0 || t.zone > 2 || t.hour < 0 || t.hour > 23 || t.minute < 0 || t.minute > 59) {
            std::cout << "skip   " << std::setw(2) << std::setfill('0') << t.hour << ':' << std::setw(2) << t.minute
                      << std::setfill(' ') << "  invalid journey ignored\n";
            continue;                                      // skip only this journey
        }
        const bool peak = isPeak(t.hour, t.minute);
        const long long f = fare(static_cast<Zone>(t.zone), peak);
        if (balance - f < MIN_BALANCE_PAISE) {
            std::cout << "card below minimum - gate stops\n";
            break;                                         // nothing more can be processed
        }
        balance -= f;
        std::cout << std::left << std::setw(7) << (t.zone == 0 ? "Inner" : t.zone == 1 ? "Middle" : "Outer")
                  << std::right << std::setw(2) << std::setfill('0') << t.hour << ':' << std::setw(2) << t.minute
                  << std::setfill(' ') << "  " << std::left << std::setw(6) << (peak ? "yes" : "no") << std::right;
        printRs(f);
        std::cout << "     ";
        printRs(balance);
        std::cout << '\n';
    }
    std::cout << "10:00 peak? " << std::boolalpha << isPeak(10, 0)
              << "   20:00 peak? " << isPeak(20, 0) << "   (both correctly off-peak)\n";
}
