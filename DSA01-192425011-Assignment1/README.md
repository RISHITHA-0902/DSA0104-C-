# DSA01-192425011-Assignment1

## Design, Implement and Validate the Fare Logic of a City Metro Smart-Card System

| | |
|---|---|
| **Name** | Rishitha |
| **Register Number** | 192425011 |
| **Course Outcome** | CO1: Object-oriented principles, C++ tokens, data types, operators and control structures |
| **Assignment** | Assignment 1 (Total Marks: 50) |
| **Compiler** | g++ (Ubuntu 13.3.0-6ubuntu2~24.04.1) 13.3.0 |
| **Language standard** | C++17 |
| **Compile command** | `g++ -std=c++17 -Wall -Wextra -o <program> <program>.cpp` |

> The timing programs (Task 05 and Task 06) are also compiled with `-O2`, as noted in their folders.

---

## Repository structure

```
DSA01-192425011-Assignment1/
├── README.md              <- this file
├── run_all.sh             <- compiles and runs every program, refreshes TaskNN/output/
├── Report/                <- assignment report (Word and PDF)
├── Task01/  Object-oriented analysis (OOP model vs procedural model)
├── Task02/  Tokens, keywords, identifiers and constants (constexpr vs #define)
├── Task03/  Data types, sizes, ranges, double vs integer paise
├── Task04/  Implicit conversions (Predict the Output)
├── Task05/  Operators and precedence (Predict the Output)
├── Task06/  Control structures (Complete the Code)
├── Task07/  Procedural vs object-oriented SmartCard
├── Task08/  Safe input and formatted output
├── Task09/  Implementation and testing
├── Task10/  Expectation vs execution (totals, overflow, warnings)
├── Task11/  Final recommendation (config-file alternative)
└── Task12/  Day-4 design challenge (Airport zone, weekend discount, monthly pass)
```

Every task folder contains:

* the C++ source files (`.cpp`, and `.hpp` where shared),
* input files (`.txt` / `.cfg`) where the program reads input,
* `output/` with the captured compile and run output (`*_output.txt`) and an output screenshot (`*_screenshot.png`),
* a `README.md` with the steps to run and a note of the result.

---

## How to compile and run

**Run everything at once (Linux / macOS / Git Bash / WSL):**

```bash
bash run_all.sh
```

**Run one program manually**, for example Task 06:

```bash
cd Task06
g++ -std=c++17 -Wall -Wextra -o task06_complete task06_complete.cpp
./task06_complete
```

On Windows (MinGW) the executable is run as `task06_complete.exe`.

Programs that read input are run with input redirection, for example:

```bash
cd Task08
g++ -std=c++17 -Wall -Wextra -o task08_safe_io task08_safe_io.cpp
./task08_safe_io < input_invalid.txt
```

---

## Fare rules used (from the scenario)

| Rule | Value |
|---|---|
| Base fare by farthest zone | Inner Rs 10, Middle Rs 20, Outer Rs 30 |
| Peak surcharge | 20% during 08:00–09:59 and 17:00–19:59 (10:00 and 20:00 are off-peak) |
| Minimum balance | At least Rs 50 must remain on the card after the fare is deducted |
| Concession (assumption) | Student 25%, Senior citizen 40% (applied after the surcharge) |
| Money representation | `long long` integer paise; percentages rounded half-up to the nearest paisa |
| Task 12 additions | Airport zone Rs 60; 10% weekend discount that replaces the peak surcharge; monthly pass for two chosen zones |

---

## Summary of results

| Task | Program(s) | Result |
|---|---|---|
| 01 | `task01_oop.cpp`, `task01_procedural.cpp` | Both styles give the same fares; only the procedural one lets a stray statement make the balance negative (−126 rupees). |
| 02 | `task02_constants.cpp`, `task02_macros.cpp` | `constexpr` + `enum class` correct (Outer peak = 3600 paise, checked by `static_assert`); the macro version gives 300000 paise because `#define PEAK_FACTOR 100 + 20` has no brackets. |
| 03 | `task03_types.cpp` | `int` is 4 bytes (max 2,147,483,647); network worst case 10,800,000,000 paise needs `long long`. `10 * 1.2 * 0.6` truncates to 719 paise instead of 720; float total for 3,000,000 fares is off by Rs 19,16,328. |
| 04 | `task04_predict.cpp`, `task04_explicit.cpp` | Output `3 3.5 / C 68 / 1 true / s >= u` explained (integer division, char→int promotion, int→bool, signed→unsigned). Brace initialisation rejects `int x{3.7}` at compile time. |
| 05 | `task05_predict.cpp`, `task05_expressions.cpp` | Output `11 16 / 9 6 4 / 6 4 / 3 1 -3 -1 / 18` explained; all 24 hours agree for the expression and `bool peak[24]` table. |
| 06 | `task06_complete.cpp` (+2) | Matches the expected output exactly. 10:00 and 20:00 are correctly off-peak. |
| 07 | `task07_procedural.cpp`, `task07_oop.cpp`, `task07_public_balance.cpp` | The private-balance `SmartCard` refuses every rule-breaking operation; the public/procedural versions allow a negative balance. Both cards are 8 bytes. |
| 08 | `task08_safe_io.cpp`, `task08_getline.cpp` | Letters, out-of-range values and wrong categories are rejected and re-asked; formatted journey report printed. |
| 09 | `task09_tests.cpp`, `task09_table_tests.cpp` | 30/30 hand-written checks and 17/17 table-driven cases pass; the 5-journey hand total Rs 90.10 matches. |
| 10 | `task10_totals.cpp`, `task10_warnings_demo.cpp` | `int` overflows at journey 1,095,975; `double` total differs by Rs 0.000056 but 114,284 single fares truncate to a wrong paisa; float is off by Rs 4,44,095.40. Five `-Wall -Wextra` warnings explained. |
| 11 | `task11_config.cpp` | Fare table loaded from `fares.cfg` in ~10–45 µs; bad or missing files fall back safely to the built-in table. |
| 12 | `task12_original_extended.cpp`, `task12_metro_v2.cpp` | The old switch returns 0 paise for Airport (warning `-Wswitch`); the redesigned version passes 19/19 tests including regression, weekend and pass cases. |

The full analysis (UML diagrams, pseudocode, test tables and complexity analyses) is in the report in `Report/`.
