// Task 11 - Alternative concept: fare table read from a configuration file at gate start-up,
// with validation and a safe fall-back to the compiled-in (constexpr) defaults.
// Compile: g++ -std=c++17 -Wall -Wextra -o task11_config task11_config.cpp
// Run    : ./task11_config fares.cfg fares_revised.cfg fares_bad.cfg missing.cfg
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>

struct FareConfig {
    long long base[3] = {1000, 2000, 3000};       // compiled-in defaults
    int peakPercent = 20, studentPercent = 25, seniorPercent = 40;
    long long minBalance = 5000;
};

bool loadConfig(const std::string& path, FareConfig& out, std::string& error)
{
    std::ifstream in(path);
    if (!in) { error = "cannot open file"; return false; }
    std::map<std::string, long long> kv;
    std::string line;
    int lineNo = 0;
    while (std::getline(in, line)) {
        ++lineNo;
        if (line.empty() || line[0] == '#') continue;
        auto eq = line.find('=');
        if (eq == std::string::npos) { error = "line " + std::to_string(lineNo) + ": missing '='"; return false; }
        try {
            kv[line.substr(0, eq)] = std::stoll(line.substr(eq + 1));
        } catch (...) {
            error = "line " + std::to_string(lineNo) + ": value is not a number";
            return false;
        }
    }
    const char* required[] = {"INNER", "MIDDLE", "OUTER", "PEAK_SURCHARGE_PERCENT",
                              "STUDENT_CONCESSION_PERCENT", "SENIOR_CONCESSION_PERCENT", "MIN_BALANCE_PAISE"};
    for (const char* key : required) {
        auto it = kv.find(key);
        if (it == kv.end()) { error = std::string("missing key ") + key; return false; }
        if (it->second < 0) { error = std::string("negative value for ") + key; return false; }
    }
    FareConfig c;
    c.base[0] = kv["INNER"]; c.base[1] = kv["MIDDLE"]; c.base[2] = kv["OUTER"];
    c.peakPercent = static_cast<int>(kv["PEAK_SURCHARGE_PERCENT"]);
    c.studentPercent = static_cast<int>(kv["STUDENT_CONCESSION_PERCENT"]);
    c.seniorPercent = static_cast<int>(kv["SENIOR_CONCESSION_PERCENT"]);
    c.minBalance = kv["MIN_BALANCE_PAISE"];
    if (!(c.base[0] < c.base[1] && c.base[1] < c.base[2])) { error = "fares must increase with zone"; return false; }
    out = c;                                       // commit only a fully valid table
    return true;
}

std::string rs(long long p)
{
    std::string s = std::to_string(p / 100) + ".";
    if (p % 100 < 10) s += "0";
    return s + std::to_string(p % 100);
}

int main(int argc, char* argv[])
{
    for (int a = 1; a < argc; ++a) {
        FareConfig cfg;
        std::string err;
        auto t0 = std::chrono::steady_clock::now();
        bool ok = loadConfig(argv[a], cfg, err);
        auto t1 = std::chrono::steady_clock::now();
        double us = std::chrono::duration<double, std::micro>(t1 - t0).count();
        std::cout << "== " << argv[a] << " : " << (ok ? "loaded" : "REJECTED (" + err + "), using built-in defaults")
                  << "  [" << std::fixed << std::setprecision(1) << us << " microseconds]\n";
        long long outerPeak = (cfg.base[2] * (100 + cfg.peakPercent) + 50) / 100;
        std::cout << "   Inner " << rs(cfg.base[0]) << "  Middle " << rs(cfg.base[1]) << "  Outer " << rs(cfg.base[2])
                  << "  Outer peak " << rs(outerPeak) << "  min balance " << rs(cfg.minBalance) << '\n';
    }
    std::cout << "Space for the table: " << sizeof(FareConfig) << " bytes\n";
}
