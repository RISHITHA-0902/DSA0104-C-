// Q4 - Complete the Code
#include <iomanip>
#include <iostream>
class Money {
    long long paise;
public:
    Money(long long rupees = 0, int p = 0) : paise(rupees * 100 + p) {}
    // TODO 1
    Money operator+(const Money& o) const { Money r; r.paise = paise + o.paise; return r; }
    // TODO 2
    bool operator==(const Money& o) const { return paise == o.paise; }
    friend std::ostream& operator<<(std::ostream& os, const Money& m) {
        long long a = m.paise < 0 ? -m.paise : m.paise;
        return os << (m.paise < 0 ? "-" : "") << "Rs " << a / 100 << '.'
                  << std::setw(2) << std::setfill('0') << a % 100 << std::setfill(' ');
    }
};
int main()
{
    Money a(120, 75), b(79, 25);
    Money c = a + b;
    std::cout << c << '\n' << std::boolalpha << (c == Money(200)) << '\n';
    std::cout << "carry test: " << Money(0, 75) + Money(0, 50) << '\n';   // Rs 1.25
}
