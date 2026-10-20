// Task 08 - Alternative concept: read every line with std::getline() and convert
// with std::stoi() inside try/catch, instead of cin >> directly.
// Compile: g++ -std=c++17 -Wall -Wextra -o task08_getline task08_getline.cpp
// Run    : ./task08_getline < input_invalid.txt
#include <iostream>
#include <stdexcept>
#include <string>

bool parseInt(const std::string& line, int lo, int hi, int& out, std::string& why)
{
    try {
        std::size_t used = 0;
        int v = std::stoi(line, &used);
        if (line.find_first_not_of(" \t\r", used) != std::string::npos) { why = "extra characters"; return false; }
        if (v < lo || v > hi) { why = "out of range"; return false; }
        out = v;
        return true;
    } catch (const std::invalid_argument&) {
        why = "not a number";
    } catch (const std::out_of_range&) {
        why = "too large for int";
    }
    return false;
}

int main()
{
    // Read the first numeric fields of input_invalid.txt (recharge, count, zone, hour)
    const struct { const char* name; int lo, hi; } fields[] = {
        {"Recharge (Rs)", 1, 10000}, {"Journeys", 1, 20}, {"Zone", 1, 3}, {"Hour", 0, 23}};
    std::string line;
    int lineNo = 0;
    for (const auto& f : fields) {
        int value = 0;
        std::string why;
        while (std::getline(std::cin, line)) {
            ++lineNo;
            if (parseInt(line, f.lo, f.hi, value, why)) break;
            std::cout << "line " << lineNo << " \"" << line << "\" rejected for " << f.name << ": " << why << '\n';
        }
        std::cout << f.name << " = " << value << "  (line " << lineNo << ")\n";
    }
    std::string big = "99999999999";
    int v = 0;
    std::string why;
    parseInt(big, 0, 100, v, why);
    std::cout << "\"" << big << "\" rejected: " << why << '\n';
    std::string mixed = "12abc";
    parseInt(mixed, 0, 100, v, why);
    std::cout << "\"" << mixed << "\" rejected: " << why << "  (cin >> would silently read 12)\n";
}
