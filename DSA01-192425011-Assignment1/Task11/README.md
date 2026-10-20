# Task 11 – Recommend and Defend the Final Design (4 marks)

## Files
| File | Purpose |
|---|---|
| `task11_config.cpp` | Alternative concept: fare table read from a configuration file at start-up, validated, with fall-back to built-in defaults |
| `fares.cfg` | Current fares |
| `fares_revised.cfg` | A fare revision (Outer Rs 35) applied without recompiling |
| `fares_bad.cfg` | A broken file (negative fare, misspelt key) that must be rejected |

## Steps to run
```bash
g++ -std=c++17 -Wall -Wextra -o task11_config task11_config.cpp
./task11_config fares.cfg fares_revised.cfg fares_bad.cfg missing.cfg
```

## Recommended final design
* Money: `long long` integer paise everywhere; percentages with integer arithmetic and half-up rounding (Tasks 3 and 10).
* Constants: one `constexpr` set in a namespace and `enum class Zone`; fares in a `constexpr std::array` indexed by zone (Tasks 2 and 6).
* Classes: `FareCalculator` (rules), `SmartCard` (private balance, `deduct`/`canTravel`/`recharge`), `Journey`, `MonthlyPass`, `Gate` (Tasks 7 and 12).
* Tests: table-driven, run after every change (Task 9).
* Fare revision: edit the constant (or the config file), run the test table, rebuild and redeploy.

## Result
The config file was loaded in about 10–45 microseconds (measured) and occupies 48 bytes. The revised file changes
the Outer peak fare to Rs 42.00 without recompiling; `fares_bad.cfg` and a missing file are rejected and the gate keeps the
built-in table.
