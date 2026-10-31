# Assignment 2 – CO2 : Assignment Report

**Name:** N Rishitha  **Register No:** 192425011
**Compiler:** g++ 13.3.0  **Standard:** C++17
**Compile command:** `g++ -std=c++17 -Wall -Wextra <file>.cpp -o <prog>`
**GitHub Repository Link:** `________________________________________`

> All predictions below were written **before** running the programs. Observed
> outputs are captured in each `Q*/output.txt` and `Q*/Q*_output.png`.

---

## Q1 – Call by Value, by Reference and by Pointer (6 Marks)
**Context:** Hostel Fee Concession Updater (merit 10 %, sports 15 %, single parent 25 %).

### Three implementations (`q1_three_ways.cpp`)
* **By value** `void applyByValue(long long fee, int %)` — `fee` is a *copy*; the caller's fee is unchanged.
* **By reference** `void applyByReference(long long& fee, int %)` — `fee` is another name for the caller's variable; the caller's fee changes.
* **By pointer** `void applyByPointer(long long* fee, int %)` — receives the address; `*fee` is the caller's variable; the caller's fee changes.

### Where each stores its parameter / why only two change the caller
A by-value parameter lives in a **new stack slot** local to the function, so writing it never touches the original. A reference parameter stores **no new object** — it is an alias (implemented as the caller's address) — and a pointer parameter stores a **copy of the address**; dereferencing it reaches the caller's object. Hence only the reference and pointer versions modify the caller's fee.

### When a `const` reference should be used
When the function only needs to **read** a large object and must not copy it — e.g. `previewAfter(const long long& fee, int)` returns a new value and guarantees the stored fee is untouched. For a big `Student` object, `const Student&` avoids both a copy and accidental modification.

### Design decision — change in place vs return the new fee
Because **several concessions may stack** on one student, I prefer a function that **returns the new fee** (`long long apply(long long fee, int %)`) rather than mutating in place. Returning keeps each call a pure calculation, makes chaining explicit (`fee = apply(apply(fee, merit), sports);`), is easy to unit-test, and avoids hidden side-effects. In-place mutation is justified only when the fee object is large and copying it repeatedly is costly.

### Predict the Output (`q1_predict.cpp`)
**My prediction (before running):**
```
1 12 13
1 100
16 13 6
```
**Actual output:** identical. **Explanation of each line:**
* `change(a,b,&c)`: `a` by value → stays **1**; `b` by reference → 2+10 = **12**; `c` by pointer → 3+10 = **13**.
* `bigger(a,c)` returns a reference to the larger of `a(1)` and `c(13)`, i.e. to `c`. `bigger(a,c)=100` writes **100 into c**; `a` stays 1 → `1 100`.
* Default arguments used by `show`: `show(1)` uses y=5,z=10 → 16; `show(1,2)` uses z=10 → 13; `show(1,2,3)` uses none → 6.
No differences between prediction and actual output.

### Complete the Code (`q1_concession.cpp`)
TODO 1 filled with `fee -= fee * percent / 100;` (call by reference).
**Expected / actual output (match):**
```
after call by value    : 45000
after call by reference: 40500
```
**Why call by value leaves the fee at 45000:** `applyByValue` shrinks only its local copy; when it returns, that copy is destroyed and `hostelFee` in `main` is untouched. `applyConcession` takes `long long&`, so it edits `main`'s variable → 45000 − 4500 = 40500.

### Alternative Concept & Complexity Analysis
If the concession **returned** the new fee instead of using a reference, `main` would write `fee = applyConcession(fee, 10);`. Time is **O(1)** in all versions (one arithmetic expression). Space copied **per call**:

| Parameter passing | `long long` (8 bytes) | large `Student` object |
|---|---|---|
| by value | copies 8 bytes | copies the whole object (expensive) |
| by reference | copies an address (8 bytes), no object copy | no object copy |
| by const reference | copies an address, no object copy, read-only | no object copy, read-only |

For a small `long long` the copy is negligible; for a large object, (const) reference avoids an expensive copy.

---

## Q2 – Return by Reference and Its Dangers (4 Marks)
**Context:** Examination Marks Register.

### Safe reference return (`q2_marks.cpp`)
`int& highest(int marks[], int n)` returns a reference **into the caller's array**, so the caller can moderate the top mark in place and even put the call on the left of `=`:
```
Before moderation: 58 72 65 90 88
After  +2 on top : 58 72 65 92 88   <-- highest(marks,5) += 2;
Top mark (read-only view): 92
```

### Returning a reference to a local — undefined behaviour (`q2_dangling.cpp`)
`int& badHighest(int,int)` returns a reference to local `top`, destroyed when the function returns.
**Compiler warning actually produced (`-Wall -Wextra`):**
```
warning: reference to local variable 'top' returned [-Wreturn-local-addr]
```
**Run-time result:** the program **segfaulted** — a concrete demonstration that reading through a dangling reference is undefined behaviour, not a guaranteed value.

### reference vs const reference vs copy
* **`int&`** — caller can read *and write* the referred object (must outlive the call).
* **`const int&`** — caller can read but *not write*; still no copy.
* **copy (`int`)** — caller gets an *independent value*; always safe, but writing it does not affect the original.

### Changed Condition — register kept in a growing `std::vector` (`q2_vector_growth.cpp`)
A reference taken before growth may dangle because `push_back` can **reallocate** and move all elements. Observed:
```
Before growth: data() = 0x...2b0, capacity = 3, topRef = 72
After  growth: data() = 0x...2e0, capacity = 6
```
The buffer address changed → the earlier `int& topRef` now points at freed memory. **Safe fix:** store an **index** and re-access `marks[1]` after growth (index stays valid across reallocation).

### Alternative Concept & Complexity Analysis
Returning an **index (`int`)** or a **pointer (`int*`)** instead of a reference:
* index: caller writes `marks[idx] += 2;` — survives reallocation.
* pointer: caller writes `*p += 2;` — same dangling risk as a reference.

Finding the highest mark is **O(n)** in all three (one linear scan); extra space is **O(1)**. **Safety when the array becomes a growing vector:** index is safest; reference and raw pointer both become invalid on reallocation.

---

## Q3 – Default Arguments and Function Overloading (6 Marks)
**Context:** Courier Charge Calculator.

### Why the original did not compile (`q3_ambiguous.cpp`)
`charge(double)` and `charge(double, bool=false, bool=false)` can **both** accept a single `double`, so `charge(2.0)` matches two candidates equally. Captured error:
```
error: call of overloaded 'charge(double)' is ambiguous
note: candidate: 'double charge(double)'
note: candidate: 'double charge(double, bool, bool)'
```

### Redesign (`q3_courier.cpp`)
Overload on the **first parameter's type** so no two candidates tie, and keep common calls short with defaults on the optional flags:
* `charge(double kg, bool express=false, bool insured=false)` — kilograms (primary).
* `charge(int grams, bool express=false, bool insured=false)` — grams, converts to kg.
* `charge(const Parcel&)` — a structured object.

### Predict which overload each of six calls selects
| Call | Predicted selection | Predicted value | Actual |
|------|--------------------|-----------------|--------|
| `charge(2)` | `int` (grams) – exact match | 0.002·30 = **0.06** | 0.06 ✓ |
| `charge(2.0)` | `double` (kg) – exact match | 2·30 = **60.00** | 60.00 ✓ |
| `charge(2.0f)` | `double` – **float promotes to double** (beats float→int conversion) | **60.00** | 60.00 ✓ |
| `charge(5.0,true)` | `double`, express | 5·50 = **250.00** | 250.00 ✓ |
| `charge(5.0,true,true)` | `double`, express+insured | 250+100 = **350.00** | 350.00 ✓ |
| `charge(Parcel{3,F,T})` | `Parcel` | 3·30+100 = **190.00** | 190.00 ✓ |

All predictions matched.

### How the compiler chooses among overloads
It ranks each argument's conversion: **exact match > promotion > standard conversion**, and the best viable candidate must win every argument with no tie. `charge(2)` is an exact `int` match; `charge(2.0f)` prefers the `double` overload because `float→double` is a floating-point **promotion** whereas `float→int` is a **conversion** (worse).
**Why default arguments must be given from the right:** arguments are bound left-to-right, so omitted ones can only be the trailing parameters; a gap in the middle (e.g. `f(int a, int b=1, int c)`) would make a call like `f(1,2)` ambiguous about which parameter `2` fills — hence defaults fill from the right only.

### Alternative Concept & Complexity Analysis
A **single `Parcel` struct with optional fields** (or a function template) could replace the overloads. Overload resolution happens **entirely at compile time (O(1) at run time, no run-time cost)**. Space copied per call is a few bytes (weight + two bools) or one small struct. A struct/template **handles a new option** (say "fragile") by adding a field — the call sites stay the same — whereas adding a bool default risks new ambiguities, so the struct design scales better as options grow.

---

## Q4 – Specifying a Class with an Array Inside It (6 Marks)
**Context:** Savings Account Mini-Statement — remember the last 10 transactions.

### Design (`q4_savings.cpp`)
`class SavingsAccount` with **private** data (`accountNo`, `balance`, `long long history[10]`, `count`, ring `head`) and **public** `deposit()`, `withdraw()`, `miniStatement()`. The constructor and `getBalance()` are defined **inside** the class; `deposit`, `withdraw`, `miniStatement`, and the private helper `record` are defined **outside** with `SavingsAccount::`.

**Observed output** (12 transactions → last 10 kept):
```
Mini-statement for A/C 192425011  (last 10 of max 10)
  CR 300 ... CR 700  DR 250  DR 400  CR 50  CR 60  CR 70
  Balance: 3330
```
The first two deposits (+100, +200) were correctly discarded.

### Why the array must be private
If `history` (or `count`) were public, any outside code could overwrite the history or desynchronise `count`, breaking the invariant "the array holds exactly `count` valid, correctly-ordered transactions." Keeping it private forces every change through `deposit`/`withdraw`, which maintain the invariant.

### 11th transaction: shift vs circular index
* **Shift:** move all 9 older entries down one slot, store the new one at the end — **O(10)** (O(KEEP)) work per transaction.
* **Circular index:** overwrite the oldest slot and advance `head = (head+1) % 10` — **O(1)** per transaction (used here).

### Design Decision — now keep the last 50
With the **circular-index** design, moving from 10 to 50 changes **only the constant `KEEP` (10 → 50)**; `add` stays O(1). A shifting design would also grow its per-transaction cost to O(50). So the circular buffer **changes least** and is the design I defend.

### Alternative Concept & Complexity Analysis
| Design | add cost | space |
|--------|----------|-------|
| shift fixed array | O(k) (k = window) | one fixed array |
| `std::deque` (`push_back`/`pop_front`) | amortised O(1) | dynamic, extra block overhead |
| circular buffer + start index | **O(1)** | one fixed array (no extra allocation) |

The circular buffer matches the deque's O(1) add while using a single fixed array — the best fit for a small, fixed window.

---

## Q5 – Static Members and Arrays of Objects (7 Marks)
**Context:** College Bus Seat Tickets.

### Class design derived from the principle
"Data belonging to the whole class exists once" → the **seat counter is a `static` data member** (`static int issued;`), defined **once outside** the class (`int BusTicket::issued = 0;`). Capacity is `static const int CAPACITY`. `count()` is a **static member function** (callable as `BusTicket::count()` without an object).
* A static member function **cannot use `this` or a non-static member** because it is not tied to any one object — there is no `this` pointer; it may only touch static members.
* An **array of objects** `BusTicket trip[4]{...}` constructs each element in turn, so the shared `issued` is updated once per construction.

### Predict the Output (`q5_predict.cpp`, Token)
**My prediction:** `101 103 104 4`
**Actual:** `101 103 104 4` (match).
**Explanation:** `Token::next` starts at **100**. Constructing `counter[3]` runs the constructor **3 times** (ids 101, 102, 103); `extra` runs it a **4th** time (id 104). So `counter[0]=101`, `counter[2]=103`, `extra=104`, and `issued()=next-100=104-100=4`. The constructor ran **4 times**.

### Complete the Code (`q5_busticket.cpp`)
TODO 1: `if (issued < CAPACITY) { ++issued; seat = issued; }`. TODO 2: `return issued;`.
**Expected / actual output (match):**
```
Asha: Seat 1
Bala: Seat 2
Chitra: Seat 3
Dev: Waiting
Issued: 3
```
Creating four tickets for a capacity-3 bus gives seats 1–3 and pushes Dev to the waiting list; `issued` stops at 3.
**Count across a function called twice:** because `issued` is static, it is **shared and persistent** — tickets created in a second function call continue from the previous value (they do not reset), which is exactly why only three seats are ever issued across the whole run.

### Alternative Concept & Complexity Analysis
A **global variable** or a separate **`TicketOffice` object** passed to every `BusTicket` could hold the counter. All are **O(1)** to read/update and use **one counter in all**. Trade-offs: a global breaks **encapsulation** (anyone can change it); a static member keeps the counter tied to the class. A passed-in `TicketOffice` is the only design that lets you **run two buses at once** (two independent counters), which a single static member cannot.

---

## Q6 – Friend Functions and Inline Functions (8 Marks)
**Context:** Train Journey Planner — `Time` class with private `h, m`.

### Why friend functions
`minutesBetween(a,b)` and `earlier(a,b)` operate on **two `Time` objects equally**, so neither "belongs" to one object. A **friend** function is called like a free function (`minutesBetween(a,b)`, not `a.minutesBetween(b)`) yet is granted access to the private `h`/`m` of **both** arguments. A member function would privilege one object (`this`) over the other and read as asymmetric.
* **Friend vs member:** a member is called on an object (`a.f(b)`), receives a hidden `this`, and is in the class scope; a friend is called as `f(a,b)`, has no `this`, is **not** a member, but may access private data because the class granted friendship.
* **Inline:** yes — these are tiny and called millions of times. `inline` *asks* the compiler to replace the call with the function body, removing call overhead; it is a request, not a command.
* **If `h,m` were public instead**, we would lose **encapsulation**: any code could set an invalid time (e.g. `m = 99`) and the class could no longer guarantee its invariants.

### Complete the Code (`q6_time.cpp`)
TODO 1: `int d = (a.h*60+a.m)-(b.h*60+b.m); return d<0?-d:d;`. TODO 2: `return (a.h*60+a.m) < (b.h*60+b.m);`.
**Expected / actual output (match):** `205 false`
(departure 9:45 = 585 min, arrival 13:10 = 790 min → |790−585| = **205**; is arrival earlier than departure? **false**.) Extra checks: across-noon `2 true`, same-time-twice `0 false` — both correct.

### Growth Investigation — 10,000,000 calls, inline vs non-inline (`q6_timing.cpp`)
Inputs varied each iteration so the call is not hoisted. Representative times (ms):

| Build | non-inline | inline |
|-------|-----------:|-------:|
| `-O0` | ~82 | ~77 |
| `-O2` | ~28 | ~19 |

**Explanation:** At **-O0** the compiler does **not** inline, so the `inline` keyword makes essentially no difference (both ~equal). At **-O2** inlining removes the per-call overhead (stack setup, call/return), so the inline build is consistently faster. Absolute numbers vary a little between runs (shared machine), but the pattern — *inline matters only once optimisation is enabled* — is stable.

### Alternative Concept & Complexity Analysis
Written as **member functions with public getters** (`a.minutesTo(b)`, `a.isEarlier(b)`) the result is identical and, **after inlining, the time per call is the same, O(1)**. Measured at -O0 and -O2 the member version tracks the friend version. The three designs differ in **encapsulation**: friend keeps data private with symmetric access; member-with-getters exposes values through getters; making data public exposes everything. I keep the **friend** design.

---

## Q7 – Diagnose, Redesign and Validate (13 Marks)
**Program chosen:** the Savings-Account transaction history from **Q4** — specifically the fixed-array version that **shifts** elements (`q7_original.cpp`).

### A. Diagnose the Limitation (2)
Keeping the last `KEEP` transactions by **shifting** moves `KEEP−1` elements on every `add`, so one add is **O(KEEP)** and processing `M` transactions is **O(M·KEEP)**. As the bank enlarges the window (last 10 → 50 → more) and as `M` grows, this becomes slow. **Evidence** (`KEEP = 2048`, `M = 1,000,000`, -O2): **≈ 108–128 ms**. The causing characteristic is the *array-shift* in `add`.

### B. Define the Design Objective (2)
Make the per-transaction cost **independent of the window size**: `add` must be **O(1)**, with **no extra heap allocation** (keep a single fixed array), while still exposing the last `KEEP` transactions oldest→newest.

### C. Design the Revised Solution (4)
Replace shifting with a **circular buffer**: keep a `head` index; `add` writes to `buf[head]`, advances `head = (head+1) % KEEP`, and caps `count` at `KEEP`. No element ever moves. Reading oldest→newest uses modular indexing from `(head − count)`.
```
class HistoryRing {
    long long buf[KEEP]; int count = 0; int head = 0;
    add(v):   buf[head]=v; head=(head+1)%KEEP; if(count<KEEP) ++count;   // O(1)
    oldest(): start=(head-count+KEEP)%KEEP; return buf[start];
    newest(): return buf[(head-1+KEEP)%KEEP];
};
```
**Extra memory:** none beyond the same fixed array plus two ints. **New trade-off:** reading in order needs modular arithmetic (O(KEEP) only when you actually print, not per add).

### D. Predict Before Implementing (1)
I predict the circular buffer removes the shift, so `add` drops from O(KEEP) to O(1). For `KEEP = 2048`, I expect roughly a **~50–60× speed-up** for 1,000,000 adds (≈ a few ms vs ~110 ms).

### E. Implement and Validate (2)
Same workload, same -O2 build, same correctness check (`oldest kept`, `newest`):

| Version | 1,000,000 adds | oldest / newest |
|---------|---------------:|-----------------|
| Original (shift array) | **≈ 108–128 ms** | 997953 / 1000000 |
| Redesign (circular buffer) | **≈ 1.9 ms** | 997953 / 1000000 |

Both keep exactly the same last-2048 window (identical `oldest`/`newest`), so behaviour is unchanged; speed-up ≈ **55–65×**, matching the prediction.

### F. Defend the Final Design (2)
The objective is met: `add` is now O(1) and independent of `KEEP`, with no extra allocation, and output is identical. **New cost:** ordered reads need modular indexing — negligible because reads (printing a mini-statement) are rare compared with adds. **Where the original could still be preferable:** if `KEEP` were tiny (say 3) and ordered iteration happened on *every* add, the simple contiguous layout has marginally better cache locality and simpler code; but for any realistic window the circular buffer wins decisively. **Conclusion:** the redesign achieves the measurable objective and is defended by the evidence above.

---

## Summary of evidence
* Predictions for Q1, Q3, Q5, Q6 were written before running and **all matched** the actual outputs.
* Real compiler messages captured: Q2 dangling-reference warning (`-Wreturn-local-addr`) and Q3 ambiguous-overload error.
* Real measurements: Q6 inline vs non-inline at -O0/-O2 (10,000,000 calls); Q7 shift vs circular buffer (1,000,000 adds, ~55–65× faster).
* Each task folder contains the source, `output.txt`, and an `*_output.png` screenshot.
