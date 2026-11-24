# DSA01 – Assignment 5 (CO5): Runtime Polymorphism and Exceptions – Hospital Billing

| | |
|---|---|
| **Name** | N Rishitha |
| **Register No.** | 192425011 |
| **GitHub** | https://github.com/RISHITHA-0902/DSA0104-C- |
| **Compiler** | g++ 13.3.0 (Ubuntu) |
| **Compile** | `g++ -std=c++17 -Wall -Wextra <file>.cpp -o <file>`; tests with `-g -fsanitize=address`; benchmarks with `-O2` |

Folders: `A1`–`A5` (design; predict-the-output programs in `A2`, `A5`), `B1`–`B4` (code). `B1/billing.hpp` holds the full hierarchy and is reused unchanged by `B2` and `B4`. Every program has a `*_output.txt` with the command and recorded output.

---

# PART A – DESIGN

## A1 – Polymorphic Bill Hierarchy
**A1.1 Common interface:** `total()`, `type()`, `package()`, `print()`; common data: patient name.
**A1.2 UML** (`{pv}` pure virtual, `{v}` virtual, `{o}` override)
```
                    «abstract» Bill
                    # patient : string
                    + total() : double {pv}
                    + type() : string {pv}
                    + package() : string {v}
                    + print()            (non-virtual, calls total/type)
                    + ~Bill() {v}
      ▲                 ▲                  ▲                  ▲
OutPatientBill     InPatientBill       DayCareBill        EmergencyBill
- consultation     - days, perDay      - pkg, price       - base, perHour, hours
- tests            - procedures, meds
+ total() {o}      + total() {o}       + total() {o}      + total() {o}
+ type() {o}       + type() {o}        + type() {o}       + type() {o}
```
**A1.3 Pseudocode**
```
ALGORITHM DayTotal(bills[0..n-1])        // bills are Bill pointers
BEGIN
  sum ← 0
  FOR i ← 0 TO n-1 DO
      sum ← sum + bills[i]→total()        // dynamic dispatch
  END FOR
  RETURN sum
END
```
**Alternative (type field + switch):** O(n) in both (one switch or one virtual call per bill). The single struct must hold the union of all fields (unused space); the virtual design adds one 8-byte vptr. A new type changes the enum + every switch vs. one new class.

## A2 – Runtime Binding
**A2.1** Early binding: call resolved at compile time from the *pointer type* (non-virtual `Bill::print`). Late binding: `b->total()` on a virtual function is resolved at run time from the *object's* vtable, so an `InPatientBill` behind a `Bill*` runs `InPatientBill::total`.
**A2.2 vtables**
```
Bill vtable           OutPatientBill vtable         InPatientBill vtable
[~Bill]               [~OutPatientBill]             [~InPatientBill]
[total = pure]        [OutPatientBill::total]       [InPatientBill::total]
[type  = pure]        [OutPatientBill::type]        [InPatientBill::type]

b ──► object [vptr | patient | ...] ──► vtable ──► slot "total" ──► correct function
```
**A2.3** `delete b` through `Bill*` must run the derived destructor first; without a virtual destructor only `~Bill` runs (UB, leaks derived members).

**Predict the Output:** `500 ~OPBill ~Bill`
`show()` is non-virtual but calls `total()` through `this`, which is a virtual call → `OPBill::total`. Without `virtual ~Bill`: g++ warns `-Wdelete-non-virtual-dtor`; output `500 ~Bill` and ASan reports `new-delete-type-mismatch (allocated 40 bytes, deallocated 8 bytes)` (`A2/predict_nonvirtual_dtor_output.txt`).
**Alternative (dynamic_cast):** up to k casts per bill (O(k)) vs. one indirect call (O(1)); vtable costs one pointer per object either way (dynamic_cast also needs RTTI/vtable).

## A3 – Payer Interface
```
          «abstract» Payer
          + amountCovered(const Bill&) : double {pv}
          + name() {pv},  patientPays(b) = b.total() − amountCovered(b)
     ▲              ▲                         ▲                    ▲
  SelfPay      Insurance                GovernmentScheme      CorporateTieUp (B4, new)
  covers 0     - coverLimit, coPayPct   - rates : map<pkg,amt> 80 %, max 25,000
               claim = total×(100−co)/100   min(rate, total)
               throws ClaimError if > limit  throws UnknownPackage
```
**A3.2 Extension:** derive a new class from `Payer` and override `amountCovered()`; no existing class or loop changes (done in `B4/extension.cpp`).
**Alternative (`std::function`):** O(1) per call in both; data such as cover limit must be captured in the lambda (closure, often heap-allocated, 32 bytes for std::function) instead of named members – harder to inspect, test and extend with more operations.

## A4 – Exception Strategy
| Error | Exception |
|---|---|
| negative charge / days | `InvalidCharge : BillingError` |
| claim above cover | `ClaimError : BillingError` |
| unknown package | `UnknownPackage : ClaimError` |
| missing patient record | `MissingRecord : BillingError` |
| non-numeric input | `std::invalid_argument` (from `std::stod`) |
| allocation failure | `std::bad_alloc` |
```
std::exception
 └─ std::runtime_error
     └─ BillingError
         ├─ InvalidCharge
         ├─ MissingRecord
         └─ ClaimError
             └─ UnknownPackage
```
**A4.2** May throw: constructors (validation), `amountCovered()`, input parsing. Must not throw (`noexcept`): destructors, `name()`, swap/move operations, logging. A destructor that throws during stack unwinding calls `std::terminate`, so destructors are implicitly `noexcept`.
**Alternative (return codes):** normal path similar; error path faster, but every caller must check and forward the status (extra parameter/return value per level, easy to ignore). Exceptions cost ~0 on the normal path and are expensive only when thrown.

## A5 – Recovery and Rethrowing
**A5.1** Low level (constructors/payers): throw only. Settlement layer: catch `ClaimError`, write an audit log, `throw;` (rethrow the same object). Top level (`main`/request loop): catch `BillingError`, then `std::exception`, then `catch(...)`, report to the user and continue with the next bill – no bill is lost.
**A5.2** Stack unwinding: when an exception leaves a function, all fully-constructed local objects are destroyed in reverse order. With RAII (`unique_ptr`, `vector`, `ofstream`) the destructors free memory/close files automatically (proved by `+/-` messages in `B2/errors_output.txt`).
**Predict the Output:** `~f ~main caught 42` – `g` in `f()` is destroyed first, then `m` as the try block is left, *before* the handler runs; control never returns to the next statement, so "not printed" is skipped.
**Alternative (delete in every catch):** same time, but each new error path needs another `delete` – every catch block is a possible leak; RAII has 0 such places.

---

# PART B – IMPLEMENTATION & EXPERIMENTS

## B1 – Polymorphic Billing
**B1.1** `B1/billing.cpp` (hand-verified):
| Bill | Calculation | Total | Insurance (10 % co-pay) | Govt scheme |
|---|---|---|---|---|
| OP Ravi | 500 + 1200 | 1700 | 1530 | 1000 (OPD rate) |
| IP Meena | 3×2500 + 40000 + 6000 | 53500 | 48150 | 50000 (IPD rate) |
| DC Arjun | package | 18000 | 16200 | 15000 (CATARACT) |
| ER Lakshmi | 3000 + 1500×4 | 9000 | 8100 | 8000 |
Day total **82200**.
**B1.2** Changes vs design: added `package()` (needed by GovernmentScheme) and validation helper `nonNegative()`.
**B1.3** `no_virtual_total.cpp`: base-pointer calls give `0 0 Total 0` instead of 49200 – static binding.
**B1.4** `no_virtual_dtor.cpp` under ASan: `new-delete-type-mismatch` (72 vs 40 bytes), `~InPatient` never runs.
**Complete the Code:** `OP Ravi: 1700 / IP Meena: 47500 / Total 49200` ✔. Removing `virtual` from `total() = 0` → `error: initializer specified for non-virtual method` (pure specifier is only allowed on virtual functions).
**Alternative (templates):** no virtual call, functions can inline; but one container per type and no single mixed list of bills.

## B2 – Engineered Error Conditions (`B2/errors.cpp`)
Negative charge → `BillingError`; claim 126000 > 50000 → `ClaimError`; `LASIK` → `UnknownPackage` (caught as ClaimError); empty name → `MissingRecord`; `stod("five hundred")` → `invalid_argument`; 8 PB request → `bad_alloc`; `throw 404` → `catch(...)`. Every `+obj` has its `-obj`, and the program prints "program still running" – the hierarchy lets each layer catch exactly the level it understands.
(ASan's allocator aborts instead of throwing on an impossible size, so this test is recorded with a normal build; B4 shows the clean ASan run.)
**Predict the Output:** `ok done caught negative [log too large] caught too large` – 100 passes; −5 throws `invalid_argument`, not matched by `process`'s handler, so it goes straight to `main`; only `out_of_range` is caught, logged and rethrown.
**Complete the Code:** matches expected output. `throw e;` throws a new copy of the **static type** (`ClaimError`) – a derived `CoverExceeded` is sliced (`rethrow_demo_output.txt`); `throw;` rethrows the original object.
**Alternative (`std::optional`)**: measured in B3 – 1M claims: optional ≈0.4–0.8 ms at all rates; exceptions 1.5 ms (0 %), 16 ms (1 %), 654 ms (50 %). optional adds 8 bytes (flag + padding) per result.

## B3 – Cost Measurements (`-O2`, `B3/bench.cpp`)
| Bills | virtual | switch | std::variant |
|---|---|---|---|
| 10 (correctness) | 1.6149e+06 | 1.6149e+06 | 1.6149e+06 – agree |
| 1,000 | 0.009 ms | 0.007 ms | 0.007 ms |
| 1,000,000 | 13.8 ms | 9.5 ms | 9.8 ms |
| 10,000,000 | 128 ms | 89 ms | 87 ms |

| Invalid | exceptions | return codes | optional |
|---|---|---|---|
| 0 % | 1.5 ms | 0.9 ms | 0.4 ms |
| 1 % | 16 ms | 0.7 ms | 0.6 ms |
| 50 % | 654 ms | 1.2 ms | 0.8 ms |

**B3.4 Trade-off:** virtual calls are ~1.4× slower than switch/variant (indirect call + separate heap objects), but all are O(n) and 10M bills take <0.13 s. Exceptions are free when nothing fails but cost ≈1.3 µs per throw (654 ms / 500k throws) → use them for *rare* errors; use return codes/optional where failure is common (bulk validation). Use virtual functions when types are open-ended; switch/variant when the set is closed and speed critical.
**Alternative (std::variant):** sizeof = 32 bytes (largest alternative 24 + tag) stored contiguously; fastest here, but adding a type edits the variant and every visitor.

## B4 – Final Engineering Decision
**B4.1** `B4/extension.cpp` adds `DialysisBill` and `CorporateTieUp` without editing `billing.hpp`; all earlier totals PASS (5/5); GovtScheme correctly rejects the unknown DIALYSIS package.
**B4.2** Run under `-fsanitize=address` with thrown/caught exceptions – no ASan report (clean).
**B4.3 Decision: suitable.** 200,000 bills/day take ≈3 ms of virtual dispatch (measured 13.8 ms per 1M); invalid bills are a small fraction so exception cost is negligible. Two bill types and three payers a year are each one new class with no change to tested code, and RAII + exceptions keep it leak-free. Bulk validation of imported data should use return codes/optional.
**Alternative (plug-in rate files):** rule interpretation per bill is slower (parse/lookup vs one indirect call) and rules take extra memory, but new tariffs need no recompilation; the risk moves to configuration errors, so rules must be validated. Hybrid: C++ classes for new *behaviour*, config files for *rates*.
