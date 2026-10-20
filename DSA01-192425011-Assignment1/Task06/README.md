# Task 06 – Design the Control Structures (4 marks)

## Files
| File | Purpose |
|---|---|
| `task06_complete.cpp` | **Complete the Code**: TODO 1 (switch + integer surcharge) and TODO 2 (minimum balance) filled; rest unchanged |
| `task06_peak_boundary.cpp` | Journeys at exactly 08:00, 10:00, 17:00, 20:00; `continue` for invalid journeys, `break` on low balance |
| `task06_array_lookup.cpp` | Alternative concept: fare array indexed by zone instead of a switch, with timing |

## Steps to run
```bash
g++ -std=c++17 -Wall -Wextra -o task06_complete task06_complete.cpp && ./task06_complete
g++ -std=c++17 -Wall -Wextra -o task06_peak_boundary task06_peak_boundary.cpp && ./task06_peak_boundary
g++ -std=c++17 -Wall -Wextra -O2 -o task06_array_lookup task06_array_lookup.cpp && ./task06_array_lookup
```

## Pseudocode (fare computation)
```
ALGORITHM ComputeFare
INPUT : zone (Inner | Middle | Outer), peak (Boolean)
OUTPUT: fare in paise
BEGIN
    SET base <- 0
    CASE zone OF
        Inner  : SET base <- BASE_PAISE[0]
        Middle : SET base <- BASE_PAISE[1]
        Outer  : SET base <- BASE_PAISE[2]
    END CASE
    IF peak = TRUE THEN
        SET base <- base + (base * PEAK_SURCHARGE_PERCENT) DIV 100
    END IF
    RETURN base
END

ALGORITHM CanTravel
INPUT : balance, fare (paise)
OUTPUT: TRUE if the journey is allowed
BEGIN
    IF balance - fare >= MIN_BALANCE_PAISE THEN RETURN TRUE
    ELSE RETURN FALSE
    END IF
END
```

## Result
Output of `task06_complete` matches the expected output exactly:
```
Fare Rs 10.00 balance Rs 90
Fare Rs 36.00 balance Rs 54
Denied (low balance) balance Rs 54
```
Boundary check: 08:00 → Rs 12.00 (peak), **10:00 → Rs 20.00 (off-peak)**, 17:00 → Rs 36.00 (peak), **20:00 → Rs 30.00 (off-peak)**.
Switch vs array lookup (100 million, -O2): about 79 ms vs 70 ms; array = 24 bytes. With a fourth zone, the array needs one new
value, while the switch needs a new `case` (g++ warns `-Wswitch` if it is forgotten).
