// Q2 - Examination Marks Register (SAFE version)
// Student: N Rishitha  |  Register No: 192425011
//
// highest() returns a reference INTO the array, so the caller can moderate
// the top mark in place and even put the call on the left of '='.

#include <iostream>

// Returns a reference to the largest element of marks[0..n-1].
int& highest(int marks[], int n)
{
    int idx = 0;
    for (int i = 1; i < n; ++i)
        if (marks[i] > marks[idx]) idx = i;
    return marks[idx];          // safe: array lives in the caller
}

// const-reference version: lets the caller read but not modify.
const int& highestReadOnly(const int marks[], int n)
{
    int idx = 0;
    for (int i = 1; i < n; ++i)
        if (marks[i] > marks[idx]) idx = i;
    return marks[idx];
}

int main()
{
    int marks[5] = {58, 72, 65, 90, 88};

    std::cout << "Before moderation: ";
    for (int m : marks) std::cout << m << ' ';
    std::cout << '\n';

    // Use the returned reference on the LEFT of '=' : +2 moderation.
    highest(marks, 5) += 2;

    std::cout << "After  +2 on top : ";
    for (int m : marks) std::cout << m << ' ';
    std::cout << '\n';

    std::cout << "Top mark (read-only view): "
              << highestReadOnly(marks, 5) << '\n';
    return 0;
}
