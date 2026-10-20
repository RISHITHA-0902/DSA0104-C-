# Task 05 – Write Correct Expressions with the Right Precedence (4 marks)

## Files
| File | Purpose |
|---|---|
| `task05_predict.cpp` | The given program, unchanged |
| `task05_expressions.cpp` | Peak check, surcharge, concession, `x++` vs `++x`, negative `/` and `%`, `bool peak[24]` alternative with timing |
| `task05_i_equals_i_plus_plus.cpp` | Why `i = i++` is a mistake (compiler warning `-Wsequence-point`) |

## Steps to run
```bash
g++ -std=c++17 -Wall -Wextra -o task05_predict task05_predict.cpp && ./task05_predict
g++ -std=c++17 -Wall -Wextra -O2 -o task05_expressions task05_expressions.cpp && ./task05_expressions
g++ -std=c++17 -Wall -Wextra -o task05_i task05_i_equals_i_plus_plus.cpp && ./task05_i
```

## Predicted output (written before running)
```
11 16
9 6 4
6 4
3 1 -3 -1
18
```

## Actual output
```
11 16
9 6 4
6 4
3 1 -3 -1
18
```
The prediction matched exactly.

| Line | Operators and order |
|---|---|
| `11 16` | `*` before `+`: 5 + (3×2) = 11; brackets first: (5+3)×2 = 16 |
| `9 6 4` | `x++` yields 5 then x becomes 6; `++y` makes y 4 and yields 4; 5 + 4 = 9 |
| `6 4` | `?:` picks x (6 > 4); `%` and `*` have equal precedence, left-to-right: (6 % 4) × 2 = 4 |
| `3 1 -3 -1` | integer division truncates toward zero; the remainder takes the sign of the dividend |
| `18` | `i += 3` gives i = 0, 3, 6, 9; k = 0+3+6+9 = 18 |

## Result
All 24 hours agree for the bracketed expression, the unbracketed expression (g++ warns `-Wparentheses`) and the
`bool peak[24]` table. Measured with -O2: 100 million checks took about 80 ms (expression) vs 62 ms (table); the table costs 24 bytes.
