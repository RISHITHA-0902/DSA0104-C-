# Task 10 – Compare Expectations with Execution (4 marks)

## Files
| File | Purpose |
|---|---|
| `task10_totals.cpp` | Totals for 20,000 / 500,000 / 3,000,000 journeys in `long long` paise, `double`, `float`, `int`; time per addition |
| `task10_warnings_demo.cpp` | Careless version of the code used to collect `-Wall -Wextra` warnings |

## Steps to run
```bash
g++ -std=c++17 -Wall -Wextra -o task10_totals task10_totals.cpp && ./task10_totals
g++ -std=c++17 -Wall -Wextra -o task10_warnings_demo task10_warnings_demo.cpp && ./task10_warnings_demo
```

## Measured totals
| Journeys | long long paise (exact) | double − exact (Rs) | float − exact (Rs) | int paise |
|---|---|---|---|---|
| 20,000 | Rs 3,91,876.10 | 2.7 × 10^-9 | −0.97 | correct |
| 500,000 | Rs 97,97,140.40 | 5.0 × 10^-8 | −1,630.40 | correct |
| 3,000,000 | Rs 5,87,82,863.40 | 5.6 × 10^-5 | −4,44,095.40 | **overflowed at journey 1,095,975** |

In `double`, **114,284** of the 3,000,000 single fares truncate to a wrong paisa (for example 7.20 → 7.19).

## Compiler warnings received
| Warning | Meaning |
|---|---|
| `-Wswitch` | enumeration value `Outer` not handled in switch → fare 0 |
| `-Wparentheses` | `&&` inside `||` without brackets – correct, but hard to read |
| `-Wunused-parameter` | `minute` ignored, so 09:59 and 09:00 are treated alike |
| `-Wsign-compare` | `int` compared with `vector::size_type` (unsigned) |
| `-Wunused-variable` | `unusedTotal` declared but never used |
Note: `int paise = rupees * 100;` gives **no** warning with `-Wall -Wextra` (it needs `-Wconversion`).
