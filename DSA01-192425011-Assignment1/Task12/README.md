# Task 12 – Day-4 Design Challenge: Re-evaluate and Adapt the Design (8 marks)

## Changed requirement
* Fourth zone: **Airport, Rs 60**
* **10% weekend discount** that does not combine with the peak surcharge
* **Monthly pass**: unlimited journeys inside two chosen zones

## Files
| File | Purpose |
|---|---|
| `task12_original_extended.cpp` | Evaluation: the Task 06 switch with only the enum extended → Airport fare 0 and a `-Wswitch` warning |
| `task12_metro_v2.cpp` | Redesign: fare table indexed by zone, separate time rule (weekend vs peak), `MonthlyPass`, `SmartCard`, `Gate`, 19 tests |

## Steps to run
```bash
g++ -std=c++17 -Wall -Wextra -o task12_original_extended task12_original_extended.cpp && ./task12_original_extended
g++ -std=c++17 -Wall -Wextra -o task12_metro_v2 task12_metro_v2.cpp && ./task12_metro_v2
```

## Assumptions
* Weekend: the 10% discount applies at all hours and the peak surcharge is never added on a weekend.
* Concession is applied after the weekend discount or peak surcharge.
* A pass covers a journey only if both the origin and the destination zones are among its two zones and the journey is in the pass month.
* The Rs 50 minimum-balance rule still applies to every paid journey.

## Decision: MODIFY
The `SmartCard` class is **retained** unchanged; the fare logic is **modified** (switch → fare table, a separate time rule);
`MonthlyPass` and `Gate` are **added**.

## Result
* Original design: Airport off-peak fare = **0 paise** (should be 6000), with `warning: enumeration value 'Airport' not handled in switch`.
* Redesigned version: **19 / 19 tests pass**: 5 regression (old fares unchanged), 3 Airport, 4 weekend
  (e.g. Airport weekend 09:00 = Rs 54.00, not Rs 64.80), 5 pass and 2 minimum-balance cases.
