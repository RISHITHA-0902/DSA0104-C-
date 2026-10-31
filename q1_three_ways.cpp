// Q1 - Hostel Fee Concession Updater
// The concession written THREE ways: by value, by reference, by pointer.
// Student: N Rishitha  |  Register No: 192425011
//
// Concessions: merit 10%, sports 15%, single parent 25%.
// We predict the stored fee after each call, then verify by running.

#include <iostream>

// (1) BY VALUE  -- works on a local copy; caller's fee is NOT changed.
void applyByValue(long long fee, int percent)
{
    fee -= fee * percent / 100;   // only the copy shrinks
}

// (2) BY REFERENCE -- 'fee' is another name for the caller's variable;
//     the caller's fee IS changed.
void applyByReference(long long& fee, int percent)
{
    fee -= fee * percent / 100;
}

// (3) BY POINTER -- we receive the address; *fee is the caller's variable;
//     the caller's fee IS changed.
void applyByPointer(long long* fee, int percent)
{
    *fee -= *fee * percent / 100;
}

// Read-only view: a const reference cannot modify the caller's fee,
// yet copies nothing (good for large objects / "just look, don't touch").
long long previewAfter(const long long& fee, int percent)
{
    return fee - fee * percent / 100;   // returns a NEW value; 'fee' untouched
}

int main()
{
    const int merit = 10, sports = 15, singleParent = 25;

    // ---- by value -------------------------------------------------------
    long long feeV = 45000;
    std::cout << "Start fee                 : " << feeV << '\n';
    applyByValue(feeV, merit);
    std::cout << "After applyByValue (merit): " << feeV
              << "   <-- unchanged (copy)\n\n";

    // ---- by reference ---------------------------------------------------
    long long feeR = 45000;
    applyByReference(feeR, merit);
    std::cout << "After applyByReference    : " << feeR << '\n';

    // ---- by pointer -----------------------------------------------------
    long long feeP = 45000;
    applyByPointer(&feeP, sports);
    std::cout << "After applyByPointer      : " << feeP << '\n';

    // ---- const reference (preview only) ---------------------------------
    long long feeC = 45000;
    long long preview = previewAfter(feeC, singleParent);
    std::cout << "Preview with single-parent: " << preview
              << "   (stored fee still " << feeC << ")\n";

    return 0;
}
