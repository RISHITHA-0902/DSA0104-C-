# DSA01 – Assignment 4 (CO4): Designing Reusable Class Hierarchies

| | |
|---|---|
| **Name** | N Rishitha |
| **Register No.** | 192425011 |
| **GitHub** | https://github.com/RISHITHA-0902/DSA0104-C- |
| **Compiler** | g++ 13.3.0 (Ubuntu) |
| **Compile** | `g++ -std=c++17 -Wall -Wextra <file>.cpp -o <file>` (sanitizer: add `-g -fsanitize=address`) |

Each `TaskN` folder has the sources and `*_output.txt` with the exact command and recorded output.

---

## Task 1 – Class Design Review (Payroll)
**1.1 Deficiencies**
1. `name, id, basic, da` repeated in Professor and LabAssistant; pay rule `basic + basic*0.42` written twice.
2. All data public – any code can set a negative salary.
3. Same information, different names: `name/id` vs `nm/code`.
4. `da` field is never used (DA is recomputed with 0.42).
5. Magic number `0.42` – a DA change needs edits in several places.
6. A new category means copying a whole class again.
7. No common type, so all staff cannot be processed in one loop/array.

**1.2 Repaired design** (`Task1/main.cpp`)
```
            «abstract» Staff
            # name : string, # id : int
            + pay() : double {pure virtual}
            + role() : string {pure virtual}
            + print()
             ▲                    ▲              ▲
     SalariedStaff          GuestFaculty       Intern (new)
     # basic, DA_RATE=0.42  - hours, - rate    - stipend
     + pay() override       + pay() override   + pay() override
       ▲            ▲
   Professor     LabAssistant
   - research
   + pay() override
```
Output: Professor 128600, LabAssistant 35500, GuestFaculty 48000, Intern 10000, total 222100 – processed in one loop.

**1.3 Defence (≤100 words):** Common data and the DA rule live once in the base classes, so a new category only writes a small derived class that overrides `pay()` and `role()`. No existing class is edited or re-tested, and the payroll loop over `Staff*` works for the new type automatically, as shown by `Intern`.

**Alternative (one class + type switch):** `pay()` is O(1) either way, but every object carries unused fields (hours/rate for professors, research for others), and a new type requires changes in the enum, the switch, the constructor and every other switch (≥3 places) versus 1 new class.

## Task 2 – University People
**2.1 Relationships**
| Relationship | Kind | Inheritance |
|---|---|---|
| Student → Person, Staff → Person | is-a | hierarchical |
| TeachingAssistant → Student + Staff | is-a | multiple → hybrid (diamond) |
| ResearchScholar → Student → Person | is-a | multilevel |
| ResearchScholar has a Fellowship | has-a | member class |

**2.2 Diamond:** without `virtual`, a TA contains two Person sub-objects (two names/IDs); `ta.name` is ambiguous and data can disagree. `virtual` inheritance keeps one shared Person, constructed by the most-derived class.
**2.3** Declarations in `Task2/main.cpp` – all bases inherited **publicly** (a Student *is a* Person to all users), virtual for Student/Staff.

**2.4 Access table (member of Person as seen in Student)**
| Person member | public inheritance | protected inheritance | private inheritance |
|---|---|---|---|
| public | public | protected | private |
| protected | protected | protected | private |
| private | not accessible | not accessible | not accessible |

**Predict the Output:** `Person Student Staff TA` – the virtual base is built once, first, by the most-derived class.
Without `virtual`: `Person Student Person Staff TA` (no compiler error; error only on use: `'Person' is an ambiguous base`).
**Alternative (composition):** sizeof TA (virtual) = 176 vs TA_Composed = 256 (two Person copies). Reaching name in composition needs `asStudent.getName()` (explicit choice, no ambiguity, O(1)); virtual inheritance reaches it through one extra pointer offset, also O(1).

## Task 3 – Constructors in Derived Classes and Member Classes
- **3.1** Student/Staff/TA pass values via initializer lists. The most-derived class must call the virtual-base constructor because only it knows the single shared Person; calls in Student/Staff are ignored for a TA.
- **3.2** `Person` has an `Address` member, `Student` has a `Course` member; values are passed through the initializer list.
- **3.3 Rule observed:** virtual bases → non-virtual bases (declaration order) → members (declaration order) → constructor body. Destruction is the exact reverse. (TA: Address, Person, Course, Student, Staff, TA … then reverse.)
- **3.4** Non-virtual version: Person built twice; sizeof(TA) 128 (virtual) → 184 (non-virtual).

**Predict the Output:** `Vehicle Engine Car | ~Car ~Engine ~Vehicle`
**Complete the Code:** `192311001 Kavya (19) Salem-636001` ✔. Swapping the list gives `-Wreorder` warnings (initialisation still follows declaration order).
**Alternative (`Address*`):** extra heap allocation per object + 8-byte pointer; class must now write a destructor (`delete`), a deep copy constructor and copy assignment (Rule of Three).

## Task 4 – Pointers to Derived Classes: Forensics
**4.1 Defects:** (1) `rate()` is not virtual → static binding, always 3.5. (2) `~Account` not virtual → `delete a` runs only `~Account`, `history` leaks. (3) `Account copy = SeniorAccount();` slices off the derived part – only the Account sub-object is copied.
**4.2 Evidence:** ASan: `new-delete-type-mismatch … size of the allocated type: 8 bytes; deallocated type: 1 bytes`. Without ASan the output shows `3.5 3.5` and only `~Account` for the senior account.
**4.3 Fix:** `fixed.cpp` – virtual `rate()`, virtual destructor, `override`, `unique_ptr` + references; prints `3.5 4`, both destructors run, ASan clean.
**4.4 Rules:** a class with virtual functions needs a virtual destructor; mark overrides with `override`; store polymorphic objects by pointer/reference, never by value.
**Predict the Output:** `3.5 4 3.5` – `rate()` bound at compile time (pointer type), `vrate()` at run time (vtable), `copy` is a sliced Account so its vptr is Account's.
**Alternative (`vector<unique_ptr>`):** same O(n) time/space; removes the leak/manual delete, but missing virtual and slicing can still occur.

## Task 5 – Pointers to Objects and `this`
**Complete the Code:** `324.00` ✔ (360 − 10%). If `add()` returned `Cart` (by value), each chained call would modify a temporary copy – only "Rice" would reach `c`.
**5.1/5.2** `main.cpp` – name, pointer, reference, new/delete; array of `Cart*` searched with pointer arithmetic → `Highest cart: C2 = 600`.
**5.3 Comparison**
| Way | Safety | Lifetime | Use when |
|---|---|---|---|
| By name | safest | scope | local object |
| Pointer `->` | can be null/dangling | not owner | optional/re-seatable link |
| Reference | never null | not owner | function parameters, aliases |
| new/delete | leak risk | manual | size/lifetime decided at run time (prefer unique_ptr) |

`this` is a pointer (`Cart* const`) to the object the member was called on; `*this` is that object itself.
**Alternative (`vector<Cart>`):** search still O(n); objects stored contiguously (each object's full size, no extra pointer), but pointers into the vector become invalid when it reallocates.

## Task 6 – Design Challenge: Vehicle Rental Fleet
**Stage 1 – Requirements:** R1 each vehicle has reg no. and daily rate; R2 motor vehicles have fuel/charge level and service date; R3 cars have seats, EVs battery and per-kWh charge; R4 renting a rented vehicle is rejected; R5 return computes charge + late fee; R6 a new type can be added with no change to existing classes.

**Stage 2 – Hierarchy**
```
              «abstract» Vehicle  (reg, dailyRate, rented)
              + charge(days) = 0, kind() = 0, lateFeePerDay() virtual
               ▲                         ▲
            Bicycle           «abstract» MotorVehicle (level, Date serviceDue ◆ member)
                                + energyCost() = 0
                           ▲             ▲               ▲
                      TwoWheeler        Car (seats)    ElectricBus (added later)
                                         ▲
                                     ElectricCar (batteryKWh)   [multilevel]
```
**Stage 3 – Decisions:** common data/rent logic in Vehicle; `charge`, `energyCost`, `kind`, `lateFeePerDay` virtual; fleet stored as `vector<unique_ptr<Vehicle>>` (base-class pointers).

**Stage 4 – Pseudocode**
```
ALGORITHM RentVehicle(fleet, reg)
  v ← FIND(fleet, reg)
  IF v = NULL OR v.rented THEN RAISE error
  v.rented ← TRUE

ALGORITHM ReturnVehicle(fleet, reg, days, lateDays)
  v ← FIND(fleet, reg)
  IF v = NULL OR NOT v.rented THEN RAISE error
  v.rented ← FALSE
  RETURN v.charge(days) + lateDays × v.lateFeePerDay()
```
**Stage 5 – Output:** Bicycle 3 days = 300; TwoWheeler 2 days = 1010 (800 + 2 L×105); Car 2 days + 1 late = 7020; ElectricCar 1 day = 2860 (2500 + 30 kWh×12); ElectricBus 1 day + 2 late = 26200 – the bus was added with **no change** to existing classes.

**Alternative (single class + switch):** O(1) per charge() in both; single class wastes battery/seats fields on bicycles, the virtual version adds only one vptr (8 bytes); a new type needs edits to the enum and every switch versus one new class.
