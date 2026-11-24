// Q4 - Full Money class for a billing counter
#include <iomanip>
#include <iostream>
#include <sstream>
class Money {
    long long paise;
public:
    Money(long long rupees = 0, int p = 0) : paise(rupees * 100 + p) {}
    static Money fromPaise(long long p) { Money m; m.paise = p; return m; }
    Money& operator+=(const Money& o) { paise += o.paise; return *this; }   // modifies, returns *this
    Money& operator-=(const Money& o) { paise -= o.paise; return *this; }
    Money operator*(int qty) const { return fromPaise(paise * qty); }         // Money * count makes sense
    friend Money operator+(Money a, const Money& b) { return a += b; }      // friend: allows 100 + m
    friend Money operator-(Money a, const Money& b) { return a -= b; }
    friend bool operator==(const Money& a, const Money& b) { return a.paise == b.paise; }
    friend bool operator<(const Money& a, const Money& b) { return a.paise < b.paise; }
    bool negative() const { return paise < 0; }
    friend std::ostream& operator<<(std::ostream& os, const Money& m) {
        long long a = m.paise < 0 ? -m.paise : m.paise;
        return os << (m.paise < 0 ? "-" : "") << "Rs " << a / 100 << '.'
                  << std::setw(2) << std::setfill('0') << a % 100 << std::setfill(' ');
    }
    friend std::istream& operator>>(std::istream& is, Money& m) {    // reads "123.45"
        long long r = 0; char dot = 0; int p = 0;
        if (is >> r >> dot >> p && dot == '.') m.paise = r * 100 + p; else is.setstate(std::ios::failbit);
        return is;
    }
};
int main() {
    std::cout << "0.75 + 0.50 = " << Money(0, 75) + Money(0, 50) << '\n';
    Money refund = Money(50) - Money(80);
    std::cout << "50 - 80 = " << refund << (refund.negative() ? "  (negative -> shown as amount due)" : "") << '\n';
    std::cout << "100 + m = " << 100 + Money(2, 50) << "   m + 100 = " << Money(2, 50) + 100 << '\n';
    // bill with operators
    Money rice(60), dal(120, 50), oil(180, 25), bill;
    bill += rice * 2; bill += dal; bill += oil;
    Money discount(20);
    Money payable = bill - discount;
    std::cout << "Bill " << bill << ", payable " << payable << ", over 300? " << std::boolalpha << (Money(300) < payable) << '\n';
    std::istringstream in("45.90"); Money read; in >> read; std::cout << "read: " << read << '\n';
    // double vs Money for 1,000,000 additions of 0.10
    double d = 0; Money m;
    for (int i = 0; i < 1000000; ++i) { d += 0.10; m += Money(0, 10); }
    std::cout << std::setprecision(17) << "double: " << d << "   Money: " << m << '\n';
}
