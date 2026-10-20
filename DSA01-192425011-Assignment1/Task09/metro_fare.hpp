// Task 09 - Complete fare logic (shared by the test programs in this folder)
#pragma once
#include <iomanip>
#include <optional>
#include <sstream>
#include <string>

namespace metro {

enum class Zone { Inner, Middle, Outer, Count };
enum class Category { General, Student, Senior };

constexpr long long BASE_PAISE[static_cast<int>(Zone::Count)] = {1000, 2000, 3000};
constexpr int PEAK_SURCHARGE_PERCENT = 20;
constexpr int STUDENT_CONCESSION_PERCENT = 25;
constexpr int SENIOR_CONCESSION_PERCENT = 40;
constexpr long long MIN_BALANCE_PAISE = 5000;
constexpr int MORNING_PEAK_START = 8 * 60, MORNING_PEAK_END = 10 * 60;
constexpr int EVENING_PEAK_START = 17 * 60, EVENING_PEAK_END = 20 * 60;

inline long long applyPercent(long long paise, int percent) { return (paise * percent + 50) / 100; }

inline bool isPeak(int hour, int minute)
{
    const int t = hour * 60 + minute;
    return (t >= MORNING_PEAK_START && t < MORNING_PEAK_END) ||
           (t >= EVENING_PEAK_START && t < EVENING_PEAK_END);
}

// Converts an external zone number (1..3) safely; nullopt for an unknown zone
inline std::optional<Zone> zoneFromNumber(int n)
{
    if (n < 1 || n > static_cast<int>(Zone::Count)) return std::nullopt;
    return static_cast<Zone>(n - 1);
}

// Parses an hour typed by the operator; nullopt for letters or out-of-range values
inline std::optional<int> parseHour(const std::string& s)
{
    if (s.empty()) return std::nullopt;
    int v = 0;
    for (char c : s) {
        if (c < '0' || c > '9') return std::nullopt;
        v = v * 10 + (c - '0');
        if (v > 23) return std::nullopt;
    }
    return v;
}

inline long long fare(Zone z, int hour, int minute, Category c)
{
    long long f = BASE_PAISE[static_cast<int>(z)];
    if (isPeak(hour, minute)) f = applyPercent(f, 100 + PEAK_SURCHARGE_PERCENT);
    if (c == Category::Student) f = applyPercent(f, 100 - STUDENT_CONCESSION_PERCENT);
    else if (c == Category::Senior) f = applyPercent(f, 100 - SENIOR_CONCESSION_PERCENT);
    return f;
}

class SmartCard {
public:
    explicit SmartCard(long long opening) : balance_{opening < 0 ? 0 : opening} {}
    bool canTravel(long long f) const { return f >= 0 && balance_ - f >= MIN_BALANCE_PAISE; }
    bool deduct(long long f) { if (!canTravel(f)) return false; balance_ -= f; return true; }
    bool recharge(long long p) { if (p <= 0) return false; balance_ += p; return true; }
    long long balance() const { return balance_; }
private:
    long long balance_;
};

inline std::string rs(long long p)
{
    std::ostringstream os;
    os << p / 100 << '.' << std::setw(2) << std::setfill('0') << p % 100;
    return os.str();
}

}  // namespace metro
