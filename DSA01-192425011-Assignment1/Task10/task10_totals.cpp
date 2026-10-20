// Task 10 - Expected vs actual: totals for 20,000 / 500,000 / 3,000,000 journeys
// accumulated in long long paise (reference), double rupees, float rupees and int paise.
// Compile: g++ -std=c++17 -Wall -Wextra -o task10_totals task10_totals.cpp
#include <chrono>
#include <climits>
#include <cstdint>
#include <iomanip>
#include <iostream>

constexpr long long BASE_PAISE[] = {1000, 2000, 3000};
constexpr double BASE_RUPEES[] = {10.0, 20.0, 30.0};

// Deterministic mix of journeys: zone cycles 0,1,2; 40% peak; 1 in 7 student, 1 in 7 senior
struct Mix { int zone; bool peak; int cat; };   // cat 0 general, 1 student, 2 senior
Mix journey(long long i) { return {static_cast<int>(i % 3), (i % 10) < 4, (i % 7 == 5) ? 1 : (i % 7 == 6) ? 2 : 0}; }

long long farePaise(const Mix& m)
{
    long long f = BASE_PAISE[m.zone];
    if (m.peak) f = (f * 120 + 50) / 100;
    if (m.cat == 1) f = (f * 75 + 50) / 100;
    if (m.cat == 2) f = (f * 60 + 50) / 100;
    return f;
}

double fareRupees(const Mix& m)          // the "first version" style: money as double
{
    double f = BASE_RUPEES[m.zone];
    if (m.peak) f *= 1.2;
    if (m.cat == 1) f *= 0.75;
    if (m.cat == 2) f *= 0.6;
    return f;
}

int main()
{
    std::cout << std::fixed;
    for (long long n : {20000LL, 500000LL, 3000000LL}) {
        long long exact = 0;
        double d = 0.0;
        float fl = 0.0f;
        std::int32_t asInt = 0;
        long long overflowAt = -1;
        long long paisaOffJourneys = 0;      // journeys whose double fare truncates to a wrong paisa
        for (long long i = 0; i < n; ++i) {
            const Mix m = journey(i);
            const long long p = farePaise(m);
            const double r = fareRupees(m);
            exact += p;
            d += r;
            fl += static_cast<float>(r);
            if (static_cast<long long>(r * 100) != p) ++paisaOffJourneys;
            std::int32_t next;
            if (__builtin_add_overflow(asInt, static_cast<std::int32_t>(p), &next) && overflowAt < 0) overflowAt = i + 1;
            asInt = static_cast<std::int32_t>(static_cast<std::uint32_t>(asInt) + static_cast<std::uint32_t>(p));  // wraps
        }
        std::cout << "========== n = " << n << " journeys ==========\n";
        std::cout << "long long paise (exact) : Rs " << exact / 100 << '.' << std::setw(2) << std::setfill('0')
                  << exact % 100 << std::setfill(' ') << '\n';
        std::cout << "double rupees           : Rs " << std::setprecision(6) << d
                  << "   difference " << std::scientific << std::setprecision(3) << d - exact / 100.0 << std::fixed << '\n';
        std::cout << "float rupees            : Rs " << std::setprecision(2) << fl
                  << "   difference " << std::setprecision(2) << static_cast<double>(fl) - exact / 100.0 << '\n';
        std::cout << "int paise               : " << asInt
                  << (overflowAt > 0 ? "   OVERFLOWED at journey " + std::to_string(overflowAt) : "   no overflow") << '\n';
        std::cout << "journeys whose double fare truncates to a wrong paisa: " << paisaOffJourneys << "\n\n";
    }
    std::cout << "INT_MAX = " << INT_MAX << " paise = Rs " << INT_MAX / 100 << '\n';

    // time per addition for each accumulator type
    using clock = std::chrono::steady_clock;
    const long long N = 50000000;
    auto ms = [](auto a, auto b) { return std::chrono::duration<double, std::milli>(b - a).count(); };
    volatile long long vl = 0; volatile double vd = 0; volatile int vi = 0;
    auto t0 = clock::now();
    for (long long i = 0; i < N; ++i) vl = vl + 2160;
    auto t1 = clock::now();
    for (long long i = 0; i < N; ++i) vd = vd + 21.6;
    auto t2 = clock::now();
    for (long long i = 0; i < N; ++i) vi = static_cast<int>(static_cast<unsigned>(vi) + 7u);
    auto t3 = clock::now();
    std::cout << std::setprecision(3) << "50,000,000 additions: long long " << ms(t0, t1) << " ms, double "
              << ms(t1, t2) << " ms, int " << ms(t2, t3) << " ms\n";
    std::cout << "sizes: int " << sizeof(int) << ", long long " << sizeof(long long) << ", double "
              << sizeof(double) << ", float " << sizeof(float) << " bytes\n";
}
