// Task 01 - Object-oriented model of the Namma Metro smart-card fare system
// Objects: Zone, FareTable, Passenger, Journey, SmartCard
// Compile: g++ -std=c++17 -Wall -Wextra -o task01_oop task01_oop.cpp
#include <iomanip>
#include <iostream>
#include <string>

// ---------- Zone: a value, not an object with behaviour ----------
enum class Zone { Inner, Middle, Outer };

// ---------- Passenger category decides the concession ----------
enum class Category { General, Student, Senior };

// ---------- FareTable: knows every fare rule, in one place ----------
class FareTable {
public:
    static constexpr long long BASE_PAISE[3] = {1000, 2000, 3000};  // Rs 10, 20, 30
    static constexpr int PEAK_SURCHARGE_PERCENT = 20;
    static constexpr int STUDENT_CONCESSION_PERCENT = 25;
    static constexpr int SENIOR_CONCESSION_PERCENT = 40;

    // Peak = 08:00-09:59 and 17:00-19:59 (start inclusive, end exclusive)
    static bool isPeak(int hour, int minute) {
        const int t = hour * 60 + minute;
        return (t >= 8 * 60 && t < 10 * 60) || (t >= 17 * 60 && t < 20 * 60);
    }

    static long long fare(Zone z, int hour, int minute, Category c) {
        long long f = BASE_PAISE[static_cast<int>(z)];
        if (isPeak(hour, minute))
            f = applyPercent(f, 100 + PEAK_SURCHARGE_PERCENT);
        if (c == Category::Student)
            f = applyPercent(f, 100 - STUDENT_CONCESSION_PERCENT);
        else if (c == Category::Senior)
            f = applyPercent(f, 100 - SENIOR_CONCESSION_PERCENT);
        return f;
    }

private:
    // integer percentage with rounding half-up to the nearest paisa
    static long long applyPercent(long long paise, int percent) {
        return (paise * percent + 50) / 100;
    }
};

// ---------- Passenger: who travels ----------
class Passenger {
public:
    Passenger(std::string name, Category c) : name_{std::move(name)}, category_{c} {}
    const std::string& name() const { return name_; }
    Category category() const { return category_; }
private:
    std::string name_;
    Category category_;
};

// ---------- SmartCard: owns the balance and protects the Rs 50 rule ----------
class SmartCard {
public:
    static constexpr long long MIN_BALANCE_PAISE = 5000;   // Rs 50 must remain

    explicit SmartCard(long long openingPaise) : balance_{openingPaise > 0 ? openingPaise : 0} {}

    long long balance() const { return balance_; }
    bool canTravel(long long farePaise) const { return balance_ - farePaise >= MIN_BALANCE_PAISE; }

    bool deduct(long long farePaise) {
        if (farePaise < 0 || !canTravel(farePaise)) return false;   // invariant kept
        balance_ -= farePaise;
        return true;
    }
    bool recharge(long long paise) {
        if (paise <= 0) return false;                                // no negative recharge
        balance_ += paise;
        return true;
    }
private:
    long long balance_;   // private: nobody outside can make it negative
};

// ---------- Journey: one trip through the gate ----------
class Journey {
public:
    Journey(Zone z, int hour, int minute) : zone_{z}, hour_{hour}, minute_{minute} {}
    long long fareFor(const Passenger& p) const {
        return FareTable::fare(zone_, hour_, minute_, p.category());
    }
    std::string describe() const {
        static const char* names[] = {"Inner", "Middle", "Outer"};
        std::string s = names[static_cast<int>(zone_)];
        s += hour_ < 10 ? " 0" : " ";
        s += std::to_string(hour_) + ":" + (minute_ < 10 ? "0" : "") + std::to_string(minute_);
        return s;
    }
private:
    Zone zone_;
    int hour_, minute_;
};

std::string rupees(long long paise) {
    std::string r = std::to_string(paise / 100) + ".";
    long long p = paise % 100;
    r += (p < 10 ? "0" : "") + std::to_string(p);
    return r;
}

int main() {
    Passenger rishitha{"Rishitha", Category::Student};
    SmartCard card{15000};   // Rs 150.00
    const Journey day[] = {{Zone::Inner, 7, 45}, {Zone::Outer, 8, 0}, {Zone::Middle, 10, 0},
                           {Zone::Outer, 19, 59}};

    std::cout << "Passenger: " << rishitha.name() << " (Student)\n";
    std::cout << "Opening balance: Rs " << rupees(card.balance()) << "\n\n";
    std::cout << std::left << std::setw(16) << "Journey" << std::setw(12) << "Fare (Rs)"
              << std::setw(10) << "Result" << "Balance (Rs)\n";
    for (const Journey& j : day) {
        long long f = j.fareFor(rishitha);
        bool ok = card.deduct(f);
        std::cout << std::left << std::setw(16) << j.describe() << std::setw(12) << rupees(f)
                  << std::setw(10) << (ok ? "OK" : "DENIED") << rupees(card.balance()) << '\n';
    }
    std::cout << "\nNegative recharge accepted? " << (card.recharge(-5000) ? "yes" : "no") << '\n';
    std::cout << "Final balance: Rs " << rupees(card.balance()) << '\n';
}
