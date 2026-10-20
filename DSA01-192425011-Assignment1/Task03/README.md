# Task 03 – Choose Data Types and Check Type Compatibility (4 marks)

## Files
| File | Purpose |
|---|---|
| `task03_types.cpp` | `sizeof` table, `<climits>` ranges, network overflow check, double vs paise precision and error growth |

## Steps to run
```bash
g++ -std=c++17 -Wall -Wextra -o task03_types task03_types.cpp && ./task03_types
```

## Data-type choices
| Value | Type | Size | Reason |
|---|---|---|---|
| fare, balance, totals | `long long` (paise) | 8 bytes | exact; range ±9.22 × 10^18 |
| hour, minute, zone number, percent | `int` | 4 bytes | small whole numbers |
| zone | `enum class Zone` | 4 bytes | type-safe, cannot mix with other enums |
| peak flag, allowed flag | `bool` | 1 byte | true / false |
| category code | `char` / `enum class` | 1 / 4 bytes | single letter G/S/C |
| journey count | `long long` | 8 bytes | 3,000,000 fits int, but totals do not |

## Result (measured)
* Network worst case per day = 3,000,000 × 3600 = **10,800,000,000 paise > INT_MAX (2,147,483,647)** → `int` overflows, `long long` is required.
* `0.1 + 0.2 == 0.3` is **false**. `10 * 1.2 * 0.6` = 7.1999999999999993, truncated to **719 paise instead of 720** – the "off by a paisa" bug from the scenario.
* Adding Rs 21.60 n times in `double`: error grows from 10^-14 (n = 10) to **3.1 × 10^-3 rupees** (n = 3,000,000). The `long long` paise total is exact.
* `float` for 3,000,000 × 21.60 gives 62,883,672.00 instead of 64,800,000.00.
