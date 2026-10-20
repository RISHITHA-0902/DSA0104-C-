// Task 07 - Procedural version: separate variables and free functions
// Compile: g++ -std=c++17 -Wall -Wextra -o task07_procedural task07_procedural.cpp
#include <iostream>

const long long MIN_BALANCE_PAISE = 5000;

bool canTravel(long long balance, long long fare) { return balance - fare >= MIN_BALANCE_PAISE; }

// The caller must remember to call canTravel() before deduct(); nothing forces it.
void deduct(long long& balance, long long fare) { balance -= fare; }

int main()
{
    long long cardA = 10000;   // Rs 100
    long long fares[] = {3600, 2000, 1000};
    for (long long f : fares) {
        if (canTravel(cardA, f)) { deduct(cardA, f); std::cout << "paid " << f << ", balance " << cardA << '\n'; }
        else std::cout << "denied " << f << ", balance " << cardA << '\n';
    }
    // A careless caller skips the check:
    deduct(cardA, 9000);
    std::cout << "after unchecked deduct(9000): balance " << cardA << " paise  <-- rule broken\n";
}
