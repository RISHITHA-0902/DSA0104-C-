# Task 07 – From Procedural to Object-Oriented (4 marks)

## Files
| File | Purpose |
|---|---|
| `task07_procedural.cpp` | Separate variables and free functions `canTravel()` / `deduct()` |
| `task07_oop.cpp` | `SmartCard` class with private balance, `deduct()`, `canTravel()`, `recharge()` |
| `task07_public_balance.cpp` | Alternative concept: balance as a public data member |

## Steps to run
```bash
g++ -std=c++17 -Wall -Wextra -o task07_procedural task07_procedural.cpp && ./task07_procedural
g++ -std=c++17 -Wall -Wextra -o task07_oop task07_oop.cpp && ./task07_oop
g++ -std=c++17 -Wall -Wextra -o task07_public_balance task07_public_balance.cpp && ./task07_public_balance
```

## Result
| Version | After an unchecked deduction of 9000 paise |
|---|---|
| Procedural | balance −3600 paise (rule broken) |
| Public member | balance −2600 paise (rule broken) |
| `SmartCard` (private) | deduction refused, balance stays 5400 paise; negative fare and negative recharge also refused |

Both card versions occupy **8 bytes** and every operation is O(1): encapsulation costs nothing at run time.
When the minimum balance changes, the OOP version changes one constant inside the class; the procedural version
depends on every caller remembering to call `canTravel()`.
