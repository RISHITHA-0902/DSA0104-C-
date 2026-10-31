// Q3 - The BROKEN design (does NOT compile) - kept as evidence.
// Student: N Rishitha  |  Register No: 192425011
//
// charge(double) and charge(double, bool=false, bool=false) both accept a
// single double argument, so charge(2.0) is AMBIGUOUS.
//
// Compile on purpose to capture the error:
//     g++ -std=c++17 q3_ambiguous.cpp -o q3_ambiguous
// Expect: error: call of overloaded 'charge(double)' is ambiguous

#include <iostream>

double charge(double weight) { return weight * 30; }
double charge(double weight, bool express) { return weight * (express ? 50 : 30); }
double charge(double weight, bool express = false, bool insured = false)
{ return weight * (express ? 50 : 30) + (insured ? 100 : 0); }

int main()
{
    std::cout << charge(2.0) << '\n';   // AMBIGUOUS: charge(double) vs charge(double,bool=,bool=)
    return 0;
}
