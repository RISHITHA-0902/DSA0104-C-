// Task 07 - Object-oriented version: SmartCard keeps the balance private
// and only allows changes through deduct() and recharge().
// Compile: g++ -std=c++17 -Wall -Wextra -o task07_oop task07_oop.cpp
#include <iostream>

class SmartCard {
public:
    static constexpr long long MIN_BALANCE_PAISE = 5000;   // change here only

    explicit SmartCard(long long openingPaise) : balance_{openingPaise < 0 ? 0 : openingPaise} {}

    bool canTravel(long long fare) const { return fare >= 0 && balance_ - fare >= MIN_BALANCE_PAISE; }

    bool deduct(long long fare)
    {
        if (!canTravel(fare)) return false;     // the check cannot be skipped
        balance_ -= fare;
        return true;
    }

    bool recharge(long long paise)
    {
        if (paise <= 0) return false;
        balance_ += paise;
        return true;
    }

    long long balance() const { return balance_; }

private:
    long long balance_;
};

int main()
{
    SmartCard card{10000};                     // Rs 100
    long long fares[] = {3600, 2000, 1000};
    for (long long f : fares)
        std::cout << (card.deduct(f) ? "paid " : "denied ") << f << ", balance " << card.balance() << '\n';

    std::cout << "deduct(9000) allowed? " << (card.deduct(9000) ? "yes" : "no")
              << ", balance " << card.balance() << '\n';
    std::cout << "deduct(-500) allowed? " << (card.deduct(-500) ? "yes" : "no") << '\n';
    std::cout << "recharge(-2000) allowed? " << (card.recharge(-2000) ? "yes" : "no") << '\n';
    // card.balance_ = -100;   // ERROR: 'long long SmartCard::balance_' is private
    std::cout << "Balance can never go below " << SmartCard::MIN_BALANCE_PAISE << " paise through this class.\n";
}
