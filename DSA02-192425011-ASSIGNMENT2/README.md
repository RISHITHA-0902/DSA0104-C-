# DSA01-192425011-Assignment2

**Assignment 2 – CO2: Functions and Object-Oriented Concepts in C++**

| | |
|---|---|
| **Name** | N Rishitha |
| **Register Number** | 192425011 |
| **Course Outcome** | CO2 (Bloom's K5 – Evaluate) |
| **Compiler** | g++ (GCC) 13.3.0 |
| **Standard** | C++17 |
| **Total Marks** | 50 |

**GitHub Repository Link:** `________________________________________`
*(paste the repo URL here and on the cover page of the report before submitting)*

---

## Exact compile command used

```bash
g++ -std=c++17 -Wall -Wextra <source.cpp> -o <program>
```

Timing benchmarks (Q6, Q7) are additionally built at two optimisation levels:

```bash
g++ -O0 <source.cpp> -o <program>      # no optimisation
g++ -O2 <source.cpp> -o <program>      # optimised
```

## Folder layout

```
DSA01-192425011-Assignment2/
├── README.md
├── ASSIGNMENT_REPORT.md        <- evidence for Q1–Q7 (predictions, outputs, analysis)
├── Q1/  q1_predict.cpp  q1_concession.cpp  q1_three_ways.cpp  output.txt  Q1_output.png
├── Q2/  q2_marks.cpp  q2_dangling.cpp  q2_vector_growth.cpp  q2_dangling_warning.txt  output.txt  Q2_output.png
├── Q3/  q3_ambiguous.cpp  q3_courier.cpp  q3_ambiguous_error.txt  output.txt  Q3_output.png
├── Q4/  q4_savings.cpp  output.txt  Q4_output.png
├── Q5/  q5_predict.cpp  q5_busticket.cpp  output.txt  Q5_output.png
├── Q6/  q6_time.cpp  q6_timing.cpp  output.txt  Q6_output.png
└── Q7/  q7_original.cpp  q7_redesigned.cpp  output.txt  Q7_output.png
```

## How to run each task

| Task | Build & run | Result (short note) |
|------|-------------|---------------------|
| **Q1** | `g++ -std=c++17 -Wall -Wextra Q1/q1_predict.cpp -o q1 && ./q1` (same for `q1_concession.cpp`, `q1_three_ways.cpp`) | Predict program prints `1 12 13 / 1 100 / 16 13 6`. Concession reduces 45000 → 40500 only via reference/pointer. |
| **Q2** | `g++ -std=c++17 -Wall -Wextra Q2/q2_marks.cpp -o q2 && ./q2` | Reference return lets `highest(...) += 2` moderate the top mark in place. `q2_dangling.cpp` compiles **with a warning** and crashes at run time (UB). `q2_vector_growth.cpp` shows reallocation invalidating a reference. |
| **Q3** | `g++ -std=c++17 Q3/q3_ambiguous.cpp` (**fails** – ambiguous), then `g++ -std=c++17 -Wall -Wextra Q3/q3_courier.cpp -o q3 && ./q3` | Redesigned overloads compile cleanly; all six predicted calls match. |
| **Q4** | `g++ -std=c++17 -Wall -Wextra Q4/q4_savings.cpp -o q4 && ./q4` | Mini-statement keeps only the last 10 of 12 transactions; members defined inside & outside the class. |
| **Q5** | `g++ -std=c++17 -Wall -Wextra Q5/q5_predict.cpp -o q5 && ./q5` (same for `q5_busticket.cpp`) | Predict prints `101 103 104 4`. BusTicket gives seats 1–3, Dev waiting, Issued 3. |
| **Q6** | `g++ -std=c++17 -Wall -Wextra Q6/q6_time.cpp -o q6 && ./q6`; timing: build `q6_timing.cpp` at -O0/-O2 with and without `-DUSE_INLINE` | Time program prints `205 false`. Inline helps only at -O2. |
| **Q7** | `g++ -O2 Q7/q7_original.cpp -o orig && g++ -O2 Q7/q7_redesigned.cpp -o ring && ./orig && ./ring` | Shifting array ≈ 110 ms vs circular buffer ≈ 2 ms for 1,000,000 adds (~55× faster). |

## Commit guidance (for the real GitHub upload)

Commit progressively — **one commit per task** (the commit history is checked; a single final upload is not accepted). Suggested messages:

```
Q1: call by value/reference/pointer + predict + complete-the-code
Q2: return by reference, dangling demo, vector invalidation
Q3: overload ambiguity + redesigned courier charge
Q4: SavingsAccount class with array inside
Q5: static members + arrays of objects (Token, BusTicket)
Q6: friend + inline functions, timing benchmark
Q7: diagnose/redesign/validate (shift array -> circular buffer)
docs: README + assignment report
```
