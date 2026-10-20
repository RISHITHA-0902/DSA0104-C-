// Task 03 - Data types, sizes, ranges and the double-vs-paise precision problem
// Compile: g++ -std=c++17 -Wall -Wextra -o task03_types task03_types.cpp
#include <climits>
#include <cmath>
#include <iomanip>
#include <iostream>

int main() {
    std::cout << "=== sizeof on this system ===\n";
    std::cout << std::left;
    std::cout << std::setw(20) << "bool" << sizeof(bool) << " byte\n";
    std::cout << std::setw(20) << "char" << sizeof(char) << " byte\n";
    std::cout << std::setw(20) << "int" << sizeof(int) << " bytes\n";
    std::cout << std::setw(20) << "unsigned int" << sizeof(unsigned int) << " bytes\n";
    std::cout << std::setw(20) << "long long" << sizeof(long long) << " bytes\n";
    std::cout << std::setw(20) << "float" << sizeof(float) << " bytes\n";
    std::cout << std::setw(20) << "double" << sizeof(double) << " bytes\n";
    auto journeys = 3000000;                         // auto deduces int
    std::cout << std::setw(20) << "auto (=3000000)" << sizeof(journeys) << " bytes (deduced int)\n";

    std::cout << "\n=== Ranges from <climits> ===\n";
    std::cout << "INT_MAX   = " << INT_MAX << '\n';
    std::cout << "UINT_MAX  = " << UINT_MAX << '\n';
    std::cout << "LLONG_MAX = " << LLONG_MAX << '\n';

    // Worst case for one day across the network: 3,000,000 journeys at the highest fare (Rs 36)
    long long worstPaise = 3000000LL * 3600;
    std::cout << "\nNetwork worst case per day = " << worstPaise << " paise\n";
    std::cout << "Fits in int?        " << (worstPaise <= INT_MAX ? "yes" : "NO - int overflows") << '\n';
    std::cout << "Fits in long long?  " << (worstPaise <= LLONG_MAX ? "yes" : "no") << '\n';

    std::cout << "\n=== Why money must not be double rupees ===\n";
    std::cout << std::setprecision(17);
    std::cout << "0.1 + 0.2        = " << 0.1 + 0.2 << '\n';
    std::cout << "0.1 + 0.2 == 0.3 ? " << std::boolalpha << (0.1 + 0.2 == 0.3) << '\n';
    double seniorPeakInner = 10 * 1.2 * 0.6;        // Rs 7.20 expected
    std::cout << "10 * 1.2 * 0.6   = " << seniorPeakInner << '\n';
    long long truncated = static_cast<long long>(seniorPeakInner * 100);   // naive conversion
    std::cout << "Truncated to paise: " << truncated << " (expected 720) -> off by one paisa\n";
    long long rounded = std::llround(seniorPeakInner * 100);               // resolution 1
    std::cout << "llround to paise : " << rounded << " (correct, but only if every step rounds)\n";
    long long exact = (1000LL * 120 / 100) * 60 / 100;                     // resolution 2: integer paise
    std::cout << "Integer paise    : " << exact << " (exact by construction)\n";

    std::cout << "\n=== Rounding error growth: adding n fares of Rs 21.60 ===\n";
    std::cout << std::setw(10) << "n" << std::setw(26) << "double total (Rs)"
              << std::setw(22) << "exact paise total" << "error (Rs)\n";
    std::cout << std::fixed;
    for (long long n : {10LL, 1000LL, 20000LL, 500000LL, 3000000LL}) {
        double d = 0.0;
        long long p = 0;
        for (long long i = 0; i < n; ++i) {
            d += 21.6;
            p += 2160;
        }
        double err = d - static_cast<double>(p) / 100.0;
        std::cout << std::setw(10) << n << std::setw(26) << std::setprecision(10) << d
                  << std::setw(22) << p << std::scientific << std::setprecision(3) << err
                  << std::fixed << '\n';
    }
    // float makes it far worse
    float f = 0.0f;
    for (int i = 0; i < 3000000; ++i) f += 21.6f;
    std::cout << "\nfloat total for 3,000,000 x 21.60 = " << std::setprecision(2) << f
              << " (exact 64800000.00)\n";
}
