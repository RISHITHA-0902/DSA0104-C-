# DSA01 – Assignment 3 (CO3): Constructors, Destructors and Operator Overloading

| | |
|---|---|
| **Name** | N Rishitha |
| **Register No.** | 192425011 |
| **GitHub** | https://github.com/RISHITHA-0902/DSA0104-C- |
| **Compiler** | g++ 13.3.0 (Ubuntu) |
| **Compile** | `g++ -std=c++17 -Wall -Wextra <file>.cpp -o <file>` |
| **Sanitizer runs** | add `-g -fsanitize=address` |

**Run any task:** `cd Q1 && g++ -std=c++17 -Wall -Wextra main.cpp -o main && ./main`
Each folder has the source files and a `*_output.txt` with the exact command and the recorded output/compiler messages (take screenshots of the same runs for the report).

---

## Q1 – Constructors and Constructor Overloading (Library Books)
Files: `main.cpp`, `no_default.cpp`, `reorder.cpp`, `explicit_err.cpp`

**Design.** One *target* constructor `Book(title, author, copies)` validates copies through `checked()` (throws for negatives). `Book(title)` and `Book()` delegate to it, so the check happens in **one place** and no invalid Book can exist.

- **Compiler default constructor:** supplied only when the class declares *no* constructor. Once any constructor is written it is not generated (`no_default.cpp` → `no matching function for call to 'Book::Book()'` for `Book shelf[3]`).
- **Initializer lists:** members are initialised directly (no default-construct-then-assign); required for `const`, reference members and members without default constructors.
- **Initialisation order:** declaration order, not list order. `reorder.cpp` gives `-Wreorder` and `copies` reads an uninitialised `reserved`.
- **State after construction:** `a = {Let Us C, Kanetkar, 5}`, `b = {Donated Atlas, Unknown, 1}`, `c = {<empty>, <none>, 0}`.
- **Negative copies:** `Book("X","Y",-3)` → `Rejected: copies cannot be negative: -3`.

**explicit:** mark single-argument constructors `explicit` when the conversion is not natural. `shelve(std::string("Gift"))` compiles without `explicit` (silent temporary Book) and fails with it (`explicit_err.cpp`).

**Alternative (default arguments):** `Book(std::string t="", std::string a="Unknown", int c=0)` – one constructor, one check, but defaults are fixed in the declaration and `Book("t")` gives copies 0 instead of 1. Time O(1), space O(1) in both; both designs validate in exactly one place (delegation achieves it, default args too).

## Q2 – Shallow vs Deep Copy (Prescriptions)
Files: `main.cpp` (Complete the Code), `shallow.cpp`, `experiment.cpp`

**Output:** `Ibuprofen ORS | Paracetamol Cetirizine | Paracetamol ORS` – matches expected; clean under ASan.

```
Shallow:  p.medicines ──┐            Deep:  p.medicines ──► [Paracetamol][ORS]
          q.medicines ──┴─► [Para][ORS]     q.medicines ──► [Paracetamol][Cetirizine]
```
- **Why it crashes:** both objects own the same pointer; the first destructor frees it, the second uses/frees it again. ASan (`shallow_output.txt`): `heap-use-after-free ... in Prescription::~Prescription()`.
- **Rule of Three:** a class that needs a destructor (owns a resource) also needs a copy constructor and copy assignment, because the default member-wise copies duplicate the pointer, not the resource.
- **Copy constructor called when:** (1) initialising from another object `Prescription q = p;` (2) passing by value (3) returning by value (when not elided).
- **Measurements:** by value = 1 copy, by const ref = 0 copies; `p = p` handled by self-check; copy times 10 → ~0.4 µs, 1,000 → ~26 µs, 100,000 → ~2.2 ms (O(n)).

**Conclusion:** rely on the compiler copy when all members manage themselves (`std::string`, `std::vector`); write your own when the class holds raw owning pointers.
**Alternative (std::vector – Rule of Zero):** no copy ctor/assignment/destructor needed; deep copy is O(n) time and O(n) space in both designs; a shallow copy is O(1) but incorrect.

## Q3 – Dynamic Constructors and Destructors (Auditorium)
Files: `main.cpp`, `leak.cpp`, `predict.cpp`

- **Order:** stack objects are destroyed in reverse order at the end of `main` (`ZeroRows` then `StackShow`); block objects at the closing `}`; heap objects exactly at `delete`.
- **Forgot delete:** destructor never runs → memory leak (LeakSanitizer reports `detected memory leaks` on a normal Linux machine; the sandbox used here disables LSan, so `leak_output.txt` only shows the print). `delete` instead of `delete[]` is undefined behaviour (ASan: `alloc-dealloc-mismatch`).
- **Destructor takes no arguments / cannot be overloaded:** it is called implicitly by the language, so there is nothing to pass and only one way to destroy.
- **Memory layout:** `map → [row0*][row1*][row2*]`, each `rowi* → [seats chars]`. Bytes = rows×8 + rows×seats: 10×20 = 280, 100×200 = 20,800, 1000×2000 = 2,008,000.
- **Zero rows:** no allocation, booking returns false, destructor safe.

**Predict the Output:** `+A +B -B copyAc use -Ac +P -P end -A`
`b` dies at block end; `use(a)` takes by value so a copy `Ac` is made and destroyed when `use` returns; `P` is destroyed at `delete`; `a` last at end of main.
**RAII conclusion:** release is automatic and certain (even with exceptions/early returns).
**Alternative:** one block `new char[rows*seats]` (index r*seats+s) → 1 allocation, no pointer overhead; `vector<vector<char>>` → rows+1 allocations but automatic cleanup. Access O(1) in all.

## Q4 – Arithmetic and Stream Operators (Money)
Files: `complete.cpp` (Complete the Code → `Rs 200.00` / `true`, carry `Rs 1.25`), `main.cpp`

- `+` returns a **new** object (operands unchanged); `+=` modifies and returns `*this` by reference (allows chaining, no copy).
- Members: `+=`, `-=` (change the left object). Friends: `+ - == <` (symmetric, allow `100 + m`), `<< >>`.
- `<<` must be non-member because its left operand is `std::ostream`, not Money.
- `a + b` → `operator+(a, b)` (friend) or `a.operator+(b)` (member).
- Negative subtraction is kept and printed as `-Rs 30.00` (amount due).
- **1,000,000 × 0.10:** double = `100000.00000133288`, Money = `Rs 100000.00` → integer paise is exact.
**Engineering decision:** Money × Money is meaningless (Rs²) – not allowed; Money × int and Money ± Money, comparisons are allowed.
**Alternative:** member `+` cannot do `100 + m`; friend form can. Named `add()` is the same O(1) time/space but less readable.

## Q5 – ++, Comparison and Subscript (Playlist)
Files: `main.cpp`, `predict.cpp`

- Postfix is distinguished by the dummy `int` parameter; it must return the **old value (copy)** so it is slower.
- `operator[]` needs a const version for const playlists and a non-const version for assignment.
- Out of range → throws `std::out_of_range` (`-1` and `size` both rejected).
- `a = t++` → `0:10`, `b = ++t` → `0:12`; roll-over `59:59 → 1:00:00`.
- 10M increments: -O0 prefix ≈19 ms vs postfix ≈43 ms; -O2 ≈ equal (copy optimised away).
**Predict the Output:** `5 7 7` – `a` gets the old value 5 (postfix copy), `b` gets 7 after the prefix increment.
**Conclusion:** `++i` avoids the copy for class types.
**Alternative:** `at(i)` throwing / unchecked `[]` – all O(1); the check costs one comparison per access (negligible at -O2).

## Q6 – Rules of Operator Overloading (Text)
File: `main.cpp` (clean under ASan)
- Cannot overload `::  .  .*  ?:  sizeof` – they work on names/types at compile time, not values.
- Precedence, associativity and arity are fixed; at least one operand must be a user type.
- `operator=` must be a member (the compiler would otherwise generate its own).
- Use a friend when the left operand is not the class (`<<`) or for symmetric operators.
- **Copies for `a + b + c`: 2** (one per `+`). Self-assignment safe.
- 100,000 concatenations: Text ≈3 s (reallocates every time, O(n²)) vs std::string ≈2 ms (geometric capacity growth, amortised O(1)).
**Conclusion:** writing Text teaches ownership, Rule of Three and copy costs; write such a class only for special memory control (embedded, custom allocators).
**Alternative:** `operator+(Text a, const Text& b){a+=b;return a;}` copies n1 then n2, reusing the temporary; std::string copies about n1+n2+n3 characters with fewer temporaries.

## Q7 – Complex Class (AC Impedance)
File: `main.cpp`
- **Data:** re, im. **Ops:** + − × ÷ == <<, magnitude. **Constructors:** default (0), real (resistor), real+imag.
- Formulas: (a+jb)(c+jd) = (ac−bd) + j(ad+bc); division multiplies by the conjugate, ÷ (c²+d²); throws `domain_error` when c²+d² = 0.
- R = 100 Ω, L = 0.5 H, C = 20 µF, f = 50 Hz → ZL = j157.080, ZC = −j159.155.
  - **Series:** 100 − j2.075, |Z| = 100.022 Ω (hand: 157.080 − 159.155 = −2.075 ✔)
  - **Parallel:** 99.993 + j0.830, |Z| = 99.997 Ω (matches `std::complex`).
- `5+5j / 0` → `Error: division by zero impedance`; std::complex gives `(inf,inf)` silently.
- **Limitation:** no polar form/phase angle and float equality uses a 1e-9 tolerance.
**Alternative (std::complex):** same O(1) time and two doubles; better tested accuracy, but division by zero must be checked by the user.
