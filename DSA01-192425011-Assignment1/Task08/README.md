# Task 08 – Read and Print Data Safely (4 marks)

## Files
| File | Purpose |
|---|---|
| `task08_safe_io.cpp` | Gate program: `cin >>` with `cin.fail()`, `cin.clear()`, `cin.ignore()`; journey report with `setw`, `setfill`, `fixed`, `setprecision` |
| `task08_getline.cpp` | Alternative concept: `std::getline()` + `std::stoi()` inside `try/catch` |
| `input_valid.txt` | Four correct journeys |
| `input_invalid.txt` | Letters, negative recharge, zone 7, minute 75, wrong category letters |

## Steps to run
```bash
g++ -std=c++17 -Wall -Wextra -o task08_safe_io task08_safe_io.cpp
./task08_safe_io < input_valid.txt
./task08_safe_io < input_invalid.txt
g++ -std=c++17 -Wall -Wextra -o task08_getline task08_getline.cpp
./task08_getline < input_invalid.txt
```

## Result
* Every invalid entry (`abc`, `-100`, `x`, `7`, `ten`, `75`, `Q`) is reported and asked again; the program never crashes or loops forever (end of input is detected).
* Valid run report: 4 journeys, total paid **Rs 87.00**, final balance Rs 113.00.
* Invalid run report: 3 journeys, total paid **Rs 51.40**, final balance Rs 148.60.
* The `getline` version also rejects `"12abc"` (which `cin >>` would silently read as 12) and `"99999999999"` (out of range for int).
