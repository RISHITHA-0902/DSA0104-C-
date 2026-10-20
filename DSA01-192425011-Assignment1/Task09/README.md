# Task 09 – Implement and Test the Fare Logic (4 marks)

## Files
| File | Purpose |
|---|---|
| `metro_fare.hpp` | Complete fare logic: constants, `isPeak`, `fare`, `zoneFromNumber`, `parseHour`, `SmartCard` |
| `task09_tests.cpp` | 30 separate hand-written checks (normal, boundary, invalid, 5-journey total) |
| `task09_table_tests.cpp` | Alternative concept: 17 table-driven cases run in a loop |

## Steps to run
```bash
g++ -std=c++17 -Wall -Wextra -o task09_tests task09_tests.cpp && ./task09_tests
g++ -std=c++17 -Wall -Wextra -o task09_table_tests task09_table_tests.cpp && ./task09_table_tests
```

## Hand calculation of the 5-journey set
| # | Journey | Fare |
|---|---|---|
| 1 | Inner, 13:00, general | 10.00 |
| 2 | Middle, 09:15 (peak), general: 20 × 1.2 | 24.00 |
| 3 | Outer, 11:00, student: 30 × 0.75 | 22.50 |
| 4 | Outer, 18:00 (peak), senior: 36 × 0.60 | 21.60 |
| 5 | Middle, 21:00, senior: 20 × 0.60 | 12.00 |
| | **Total** | **90.10** |
Opening balance Rs 200.00 → closing balance **Rs 109.90**.

## Result
**30 / 30** hand-written checks pass and **17 / 17** table cases pass. Boundary results: 08:00 peak, 10:00 off-peak,
17:00 peak, 20:00 off-peak; Rs 80 − Rs 30 leaves exactly Rs 50 and is allowed; Rs 79.99 − Rs 30 is denied.
Unknown zone, negative recharge, negative fare and hours typed as letters are all rejected.
