// Task 12 - Redesigned fare system:
//   * 4 zones (Airport Rs 60) in a constexpr fare table indexed by an enum class
//   * 10% weekend discount that replaces (does not combine with) the peak surcharge
//   * MonthlyPass: unlimited journeys inside two chosen zones for one calendar month
// Compile: g++ -std=c++17 -Wall -Wextra -o task12_metro_v2 task12_metro_v2.cpp
#include <algorithm>
#include <array>
#include <iomanip>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>

namespace metro {

enum class Zone { Inner, Middle, Outer, Airport, Count };
enum class Category { General, Student, Senior };
enum class DayType { Weekday, Weekend };

constexpr std::size_t ZONE_COUNT = static_cast<std::size_t>(Zone::Count);
constexpr std::array<long long, ZONE_COUNT> BASE_PAISE = {1000, 2000, 3000, 6000};
constexpr int PEAK_SURCHARGE_PERCENT = 20;
constexpr int WEEKEND_DISCOUNT_PERCENT = 10;
constexpr int STUDENT_CONCESSION_PERCENT = 25;
constexpr int SENIOR_CONCESSION_PERCENT = 40;
constexpr long long MIN_BALANCE_PAISE = 5000;
static_assert(BASE_PAISE.size() == ZONE_COUNT, "every zone needs a fare");

constexpr long long applyPercent(long long p, int percent) { return (p * percent + 50) / 100; }

constexpr bool isPeak(int hour, int minute)
{
    const int t = hour * 60 + minute;
    return (t >= 8 * 60 && t < 10 * 60) || (t >= 17 * 60 && t < 20 * 60);
}

struct Journey {
    Zone from, to;
    int hour, minute;
    DayType day;
    int month;            // 1..12, used to check a monthly pass
    Category category;
    Zone farthest() const { return std::max(from, to); }
};

class FareCalculator {
public:
    static long long fare(const Journey& j)
    {
        long long f = BASE_PAISE[static_cast<std::size_t>(j.farthest())];
        // Time rule: exactly one of discount / surcharge / nothing applies
        if (j.day == DayType::Weekend)
            f = applyPercent(f, 100 - WEEKEND_DISCOUNT_PERCENT);
        else if (isPeak(j.hour, j.minute))
            f = applyPercent(f, 100 + PEAK_SURCHARGE_PERCENT);
        // Passenger rule
        if (j.category == Category::Student) f = applyPercent(f, 100 - STUDENT_CONCESSION_PERCENT);
        else if (j.category == Category::Senior) f = applyPercent(f, 100 - SENIOR_CONCESSION_PERCENT);
        return f;
    }
};

class MonthlyPass {
public:
    MonthlyPass(Zone a, Zone b, int month) : a_{a}, b_{b}, month_{month} {}
    bool valid() const { return a_ != b_ && month_ >= 1 && month_ <= 12; }
    bool covers(const Journey& j) const
    {
        auto in = [this](Zone z) { return z == a_ || z == b_; };
        return valid() && j.month == month_ && in(j.from) && in(j.to);
    }
private:
    Zone a_, b_;
    int month_;
};

class SmartCard {
public:
    explicit SmartCard(long long opening) : balance_{opening < 0 ? 0 : opening} {}
    bool canTravel(long long f) const { return f >= 0 && balance_ - f >= MIN_BALANCE_PAISE; }
    bool deduct(long long f) { if (!canTravel(f)) return false; balance_ -= f; return true; }
    bool recharge(long long p) { if (p <= 0) return false; balance_ += p; return true; }
    long long balance() const { return balance_; }
    void attachPass(const MonthlyPass& p) { pass_ = p; }
    const std::optional<MonthlyPass>& pass() const { return pass_; }
private:
    long long balance_;
    std::optional<MonthlyPass> pass_;
};

struct GateResult { bool allowed; bool byPass; long long charged; };

class Gate {
public:
    static GateResult process(SmartCard& card, const Journey& j)
    {
        if (card.pass() && card.pass()->covers(j)) return {true, true, 0};
        const long long f = FareCalculator::fare(j);
        if (!card.deduct(f)) return {false, false, 0};
        return {true, false, f};
    }
};

inline std::string rs(long long p)
{
    std::ostringstream os;
    os << p / 100 << '.' << std::setw(2) << std::setfill('0') << p % 100;
    return os.str();
}

}  // namespace metro

using namespace metro;

int passed = 0, failed = 0;
void check(const char* id, const std::string& what, const std::string& expected, const std::string& actual)
{
    bool ok = expected == actual;
    ok ? ++passed : ++failed;
    std::cout << std::left << std::setw(5) << id << std::setw(50) << what << std::setw(10) << expected
              << std::setw(10) << actual << (ok ? "PASS" : "FAIL") << '\n';
}

int main()
{
    const auto W = DayType::Weekday, E = DayType::Weekend;
    const auto G = Category::General, S = Category::Student, C = Category::Senior;
    std::cout << std::left << std::setw(5) << "ID" << std::setw(50) << "Test case" << std::setw(10) << "Expected"
              << std::setw(10) << "Actual" << "Result\n" << std::string(80, '-') << '\n';

    // ---- Regression: old behaviour must be unchanged on weekdays ----
    check("R1", "Inner weekday off-peak", "10.00", rs(FareCalculator::fare({Zone::Inner, Zone::Inner, 12, 0, W, 10, G})));
    check("R2", "Middle weekday peak 18:15", "24.00", rs(FareCalculator::fare({Zone::Inner, Zone::Middle, 18, 15, W, 10, G})));
    check("R3", "Outer weekday peak 08:00", "36.00", rs(FareCalculator::fare({Zone::Outer, Zone::Inner, 8, 0, W, 10, G})));
    check("R4", "Outer weekday 10:00 (off-peak)", "30.00", rs(FareCalculator::fare({Zone::Outer, Zone::Outer, 10, 0, W, 10, G})));
    check("R5", "Outer weekday peak senior", "21.60", rs(FareCalculator::fare({Zone::Inner, Zone::Outer, 18, 0, W, 10, C})));

    // ---- New zone ----
    check("A1", "Airport weekday off-peak", "60.00", rs(FareCalculator::fare({Zone::Inner, Zone::Airport, 12, 0, W, 10, G})));
    check("A2", "Airport weekday peak 17:00", "72.00", rs(FareCalculator::fare({Zone::Airport, Zone::Middle, 17, 0, W, 10, G})));
    check("A3", "Airport weekday peak student", "54.00", rs(FareCalculator::fare({Zone::Airport, Zone::Inner, 9, 0, W, 10, S})));

    // ---- Weekend discount (never combined with surcharge) ----
    check("D1", "Outer weekend off-peak (30 x 0.9)", "27.00", rs(FareCalculator::fare({Zone::Inner, Zone::Outer, 13, 0, E, 10, G})));
    check("D2", "Airport weekend at 09:00 peak hour (60 x 0.9)", "54.00", rs(FareCalculator::fare({Zone::Inner, Zone::Airport, 9, 0, E, 10, G})));
    check("D3", "Outer weekend student (30 x 0.9 x 0.75)", "20.25", rs(FareCalculator::fare({Zone::Outer, Zone::Outer, 18, 0, E, 10, S})));
    check("D4", "Airport weekend senior (60 x 0.9 x 0.6)", "32.40", rs(FareCalculator::fare({Zone::Airport, Zone::Airport, 11, 0, E, 10, C})));

    // ---- Monthly pass (Inner + Middle, October) ----
    SmartCard card{20000};
    card.attachPass(MonthlyPass{Zone::Inner, Zone::Middle, 10});
    GateResult r1 = Gate::process(card, {Zone::Inner, Zone::Middle, 8, 30, W, 10, G});
    check("P1", "Pass: Inner->Middle in October is free", "0.00", rs(r1.charged));
    check("P2", "Pass: balance unchanged after P1", "200.00", rs(card.balance()));
    GateResult r2 = Gate::process(card, {Zone::Middle, Zone::Outer, 12, 0, W, 10, G});
    check("P3", "Pass: Middle->Outer is outside the pass", "30.00", rs(r2.charged));
    GateResult r3 = Gate::process(card, {Zone::Inner, Zone::Inner, 12, 0, W, 11, G});
    check("P4", "Pass: November journey, pass expired", "10.00", rs(r3.charged));
    check("P5", "Pass with two identical zones is invalid", "invalid",
          MonthlyPass{Zone::Inner, Zone::Inner, 10}.valid() ? "valid" : "invalid");

    // ---- Minimum balance with the new zone ----
    SmartCard c1{11000}, c2{10999};
    check("M1", "Rs 110 - Airport Rs 60 leaves Rs 50: allowed", "yes",
          Gate::process(c1, {Zone::Inner, Zone::Airport, 12, 0, W, 10, G}).allowed ? "yes" : "no");
    check("M2", "Rs 109.99 - Airport Rs 60: denied", "no",
          Gate::process(c2, {Zone::Inner, Zone::Airport, 12, 0, W, 10, G}).allowed ? "yes" : "no");

    std::cout << std::string(80, '-') << "\nPassed " << passed << ", failed " << failed << '\n';
    std::cout << "Fare table: " << sizeof(BASE_PAISE) << " bytes for " << ZONE_COUNT << " zones; "
              << "sizeof(SmartCard) = " << sizeof(SmartCard) << " bytes\n";
    return failed ? 1 : 0;
}
