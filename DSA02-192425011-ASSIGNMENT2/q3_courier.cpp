// Q3 - Courier Charge Calculator (REDESIGNED - compiles, no ambiguity)
// Student: N Rishitha  |  Register No: 192425011
//
// Rates: base 30/kg, express 50/kg, insurance flat +100.
//
// Design:
//   * Common calls stay SHORT via default arguments (express, insured).
//   * Different KINDS of input are separated by OVERLOADING on the first
//     parameter's TYPE, so no two candidates tie:
//        - int    -> weight given in grams
//        - double -> weight given in kilograms
//        - Parcel -> a structured object
//   * Because the overloads differ in the FIRST parameter type, the later
//     defaulted bool parameters never create an ambiguous single-argument call.

#include <iostream>
#include <iomanip>

struct Parcel {
    double kg;
    bool express = false;
    bool insured = false;
};

// (A) kilograms as double  (the "primary" overload; carries the defaults)
double charge(double kg, bool express = false, bool insured = false)
{
    return kg * (express ? 50 : 30) + (insured ? 100 : 0);
}

// (B) grams as int  (converts to kg, then reuses the rule)
double charge(int grams, bool express = false, bool insured = false)
{
    return charge(grams / 1000.0, express, insured);
}

// (C) a Parcel object
double charge(const Parcel& p)
{
    return charge(p.kg, p.express, p.insured);
}

int main()
{
    std::cout << std::fixed << std::setprecision(2);

    // Six predicted calls -------------------------------------------------
    std::cout << "charge(2)              = " << charge(2)              << "  [int -> grams]\n";
    std::cout << "charge(2.0)            = " << charge(2.0)            << "  [double -> kg]\n";
    std::cout << "charge(2.0f)           = " << charge(2.0f)           << "  [float promotes to double]\n";
    std::cout << "charge(5.0, true)      = " << charge(5.0, true)      << "  [kg, express]\n";
    std::cout << "charge(5.0, true, true)= " << charge(5.0, true, true)<< "  [kg, express, insured]\n";
    Parcel p{3.0, false, true};
    std::cout << "charge(Parcel{3,F,T})  = " << charge(p)              << "  [Parcel object]\n";
    return 0;
}
