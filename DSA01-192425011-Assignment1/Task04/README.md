# Task 04 – Control Implicit Conversions (4 marks)

## Files
| File | Purpose |
|---|---|
| `task04_predict.cpp` | The given program, unchanged |
| `task04_explicit.cpp` | Corrected version: brace initialisation + `static_cast` everywhere |
| `task04_narrowing_error.cpp` | Evidence: brace initialisation rejects narrowing (expected to fail to compile) |

## Steps to run
```bash
g++ -std=c++17 -Wall -Wextra -o task04_predict task04_predict.cpp && ./task04_predict
g++ -std=c++17 -Wall -Wextra -o task04_explicit task04_explicit.cpp && ./task04_explicit
g++ -std=c++17 -Wall -Wextra -c task04_narrowing_error.cpp     # error expected
```

## Predicted output (written before running)
```
3 3.5
C 68
1 true
s >= u
```

## Actual output
```
3 3.5
C 68
1 true
s >= u
```
The prediction matched exactly.

| Line | Conversion that took place |
|---|---|
| `3 3.5` | `a / b` is **integer division** (7/2 = 3), then int→double gives 3.0, printed as `3`. In `d`, `a` is cast first, so 7.0/2 → 2 is converted to double → 3.5. |
| `C 68` | `'A' + 2`: char promoted to int (65 + 2 = 67), converted back to char `'C'`. `ch + 1` is **char→int promotion** → 68 printed as a number. |
| `1 true` | **int→bool**: any non-zero value becomes `true`; printed as 1, then as `true` with `boolalpha`. |
| `s >= u` | **signed→unsigned**: `-1` is converted to 4,294,967,295, which is greater than 1. g++ warns `-Wsign-compare`. |

## Result
The corrected program prints `3 3.5 / C D / 1 true / s < u`. `int fareB{3.7};` is rejected with
`error: narrowing conversion ... [-Wnarrowing]`, so the mistake is caught at compile time instead of at run time.
Running time and memory are unchanged: `static_cast` generates the same instructions as the implicit conversion.
