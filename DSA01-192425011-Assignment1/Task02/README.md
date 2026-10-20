# Task 02 – Use Tokens, Keywords, Identifiers and Constants Correctly (3 marks)

## Files
| File | Purpose |
|---|---|
| `task02_constants.cpp` | One set of `constexpr` constants in `namespace fare`, `enum class Zone`, `static_assert` checks, token breakdown |
| `task02_macros.cpp` | Alternative concept: the same constants as `#define` macros, showing the bracket bug |

## Steps to run
```bash
g++ -std=c++17 -Wall -Wextra -o task02_constants task02_constants.cpp && ./task02_constants
g++ -std=c++17 -Wall -Wextra -o task02_macros task02_macros.cpp && ./task02_macros
```

## Result
* `constexpr` peak fare for Outer = **3600 paise**, computed and verified at compile time with `static_assert`.
* Plain `enum`: `OUTER == BLUE` compiles and is `true` (compiler only warns `-Wenum-compare`). With `enum class` the same comparison is a compile error.
* Macro version: `OUTER_FARE * PEAK_FACTOR / 100` expands to `3000 * 100 + 20 / 100` = **300000 paise** instead of 3600; `SQUARE(1 + 1)` = 3 instead of 4.
* Running time is identical (both are replaced by constants at compile time); `constexpr` scalars normally occupy no storage unless their address is taken.
