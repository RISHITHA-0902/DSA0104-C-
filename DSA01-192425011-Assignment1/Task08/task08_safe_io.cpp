// Task 08 - Safe input with cin >>, cin.fail(), cin.clear(), cin.ignore()
// and a formatted journey report with <iomanip>.
// Compile: g++ -std=c++17 -Wall -Wextra -o task08_safe_io task08_safe_io.cpp
// Run    : ./task08_safe_io < input_invalid.txt
#include <cctype>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>

constexpr long long BASE_PAISE[] = {1000, 2000, 3000};
constexpr int PEAK_SURCHARGE_PERCENT = 20;
constexpr int STUDENT_CONCESSION_PERCENT = 25;
constexpr int SENIOR_CONCESSION_PERCENT = 40;
constexpr long long MIN_BALANCE_PAISE = 5000;

int readInt(const std::string& prompt, int lo, int hi)
{
    while (true) {
        std::cout << prompt;
        int v;
        if (std::cin >> v) {
            if (v >= lo && v <= hi) {
                std::cout << v << '\n';                       // echo accepted value
                return v;
            }
            std::cout << v << "  -> out of range (" << lo << " to " << hi << "), try again\n";
        } else {
            if (std::cin.eof()) { std::cout << "\nEnd of input.\n"; std::exit(1); }
            std::cin.clear();                                 // 1. reset the fail state
            std::string bad;
            std::cin >> bad;                                  // 2. read the bad word to show it
            std::cout << bad << "  -> not a number, try again\n";
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');   // 3. discard rest of line
    }
}

char readCategory()
{
    while (true) {
        std::cout << "Category (G/S/C): ";
        char c;
        if (!(std::cin >> c)) { std::cout << "\nEnd of input.\n"; std::exit(1); }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
        if (c == 'G' || c == 'S' || c == 'C') { std::cout << c << '\n'; return c; }
        std::cout << c << "  -> use G (general), S (student) or C (senior citizen)\n";
    }
}

long long pct(long long p, int percent) { return (p * percent + 50) / 100; }

std::string money(long long paise)
{
    std::ostringstream os;
    os << paise / 100 << '.' << std::setw(2) << std::setfill('0') << paise % 100;
    return os.str();
}

int main()
{
    std::cout << "=== Namma Metro Gate ===\n";
    long long balance = 100LL * readInt("Recharge amount in Rs: ", 1, 10000);
    int n = readInt("Number of journeys: ", 1, 20);

    struct Row { int zone, hour, minute; char cat; long long fare; bool ok; long long bal; };
    Row rows[20];
    for (int i = 0; i < n; ++i) {
        std::cout << "-- Journey " << i + 1 << " --\n";
        Row r{};
        r.zone = readInt("Zone (1 Inner, 2 Middle, 3 Outer): ", 1, 3);
        r.hour = readInt("Hour (0-23): ", 0, 23);
        r.minute = readInt("Minute (0-59): ", 0, 59);
        r.cat = readCategory();
        const int t = r.hour * 60 + r.minute;
        const bool peak = (t >= 480 && t < 600) || (t >= 1020 && t < 1200);
        long long f = BASE_PAISE[r.zone - 1];
        if (peak) f = pct(f, 100 + PEAK_SURCHARGE_PERCENT);
        if (r.cat == 'S') f = pct(f, 100 - STUDENT_CONCESSION_PERCENT);
        if (r.cat == 'C') f = pct(f, 100 - SENIOR_CONCESSION_PERCENT);
        r.fare = f;
        r.ok = balance - f >= MIN_BALANCE_PAISE;
        if (r.ok) balance -= f;
        r.bal = balance;
        rows[i] = r;
    }

    static const char* zoneName[] = {"Inner", "Middle", "Outer"};
    std::cout << "\n" << std::string(56, '=') << '\n';
    std::cout << "                  JOURNEY REPORT\n" << std::string(56, '=') << '\n';
    std::cout << std::left << std::setw(4) << "No" << std::setw(8) << "Zone" << std::setw(7) << "Time"
              << std::setw(5) << "Cat" << std::right << std::setw(10) << "Fare(Rs)" << std::setw(9) << "Status"
              << std::setw(13) << "Balance(Rs)" << '\n' << std::string(56, '-') << '\n';
    long long total = 0;
    for (int i = 0; i < n; ++i) {
        const Row& r = rows[i];
        std::ostringstream tm;
        tm << std::setw(2) << std::setfill('0') << r.hour << ':' << std::setw(2) << r.minute;
        std::cout << std::left << std::setw(4) << i + 1 << std::setw(8) << zoneName[r.zone - 1] << std::setw(7)
                  << tm.str() << std::setw(5) << r.cat << std::right << std::setw(10) << money(r.fare)
                  << std::setw(9) << (r.ok ? "OK" : "DENIED") << std::setw(13) << money(r.bal) << '\n';
        if (r.ok) total += r.fare;
    }
    std::cout << std::string(56, '-') << '\n';
    // fixed + setprecision shown for a derived (display-only) double value
    std::cout << "Total paid: Rs " << money(total) << "   Average fare: Rs " << std::fixed
              << std::setprecision(2) << (n ? static_cast<double>(total) / 100.0 / n : 0.0) << '\n';
}
