# Task 01 – Understand the Problem and Identify the Objects (4 marks)

## Files
| File | Purpose |
|---|---|
| `task01_oop.cpp` | Object-oriented model: `Zone`, `Category`, `FareTable`, `Passenger`, `SmartCard`, `Journey` |
| `task01_procedural.cpp` | Alternative concept: same logic with global variables and free functions |
| `output/` | Captured output and screenshots |

## Steps to run
```bash
g++ -std=c++17 -Wall -Wextra -o task01_oop task01_oop.cpp && ./task01_oop
g++ -std=c++17 -Wall -Wextra -o task01_procedural task01_procedural.cpp && ./task01_procedural
```

## Objects identified
| Object | Data | Behaviour |
|---|---|---|
| Zone (`enum class`) | Inner, Middle, Outer | value only, used as an index |
| FareTable | base fares, surcharge %, concession % | `isPeak()`, `fare()` |
| Passenger | name, category | `category()` |
| Journey | zone, hour, minute | `fareFor(passenger)` |
| SmartCard | balance (private, paise) | `canTravel()`, `deduct()`, `recharge()` |

## Rules every journey must satisfy
1. Balance after the journey is at least Rs 50 (5000 paise); otherwise the gate denies entry and the balance is unchanged.
2. Fare = base fare of the farthest zone; +20% only for 08:00–09:59 and 17:00–19:59.
3. All money is integer paise; percentages are rounded half-up to one paisa.
4. Recharges and fares are never negative.

## Result
Both versions print identical fares (Rs 7.50, 27.00, 15.00, 27.00 for a student) and the same final balance Rs 73.50.
The procedural version then shows that one stray statement `g_balance -= 20000;` makes the balance **−126 rupees**,
which the `SmartCard` class makes impossible because `balance_` is private. Time per journey and memory per card are O(1) in both.
