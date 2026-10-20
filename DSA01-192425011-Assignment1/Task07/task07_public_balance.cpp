// Task 07 - Alternative concept: balance as a PUBLIC data member.
// Same time and space as the private version, but any code can break the Rs 50 rule.
// Compile: g++ -std=c++17 -Wall -Wextra -o task07_public_balance task07_public_balance.cpp
#include <iostream>

struct SmartCardPublic {
    static constexpr long long MIN_BALANCE_PAISE = 5000;
    long long balance;                                   // public
    bool canTravel(long long f) const { return balance - f >= MIN_BALANCE_PAISE; }
    bool deduct(long long f) { if (!canTravel(f)) return false; balance -= f; return true; }
};

struct SmartCardPrivate {                                // for the size comparison
private:
    long long balance_ = 0;
public:
    long long balance() const { return balance_; }
};

int main()
{
    SmartCardPublic card{10000};
    card.deduct(3600);
    std::cout << "after deduct(3600): " << card.balance << '\n';
    card.balance -= 9000;                                // allowed - no check at all
    std::cout << "after card.balance -= 9000: " << card.balance << "  <-- negative balance\n";
    std::cout << "sizeof(public card) = " << sizeof(SmartCardPublic)
              << " bytes, sizeof(private card) = " << sizeof(SmartCardPrivate) << " bytes\n";
}
