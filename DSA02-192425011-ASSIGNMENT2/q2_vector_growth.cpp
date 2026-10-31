// Q2 - Changed Condition: register now kept in a std::vector that may grow.
// Student: N Rishitha  |  Register No: 192425011
//
// A reference (or pointer/iterator) obtained BEFORE the vector grows may
// become invalid, because push_back can reallocate the internal buffer and
// move every element to a new address. The old reference then dangles.
//
// This program prints the buffer address before and after growth so you can
// SEE the reallocation. Compare the two "data() =" lines.

#include <iostream>
#include <vector>

int main()
{
    std::vector<int> marks = {58, 72, 65};
    marks.reserve(3);                 // capacity exactly 3, so next add reallocates

    int& topRef = marks[1];           // reference to the current highest (72)
    std::cout << "Before growth: data() = " << (void*)marks.data()
              << ", capacity = " << marks.capacity()
              << ", topRef = " << topRef << '\n';

    marks.push_back(90);              // capacity exceeded -> reallocation likely

    std::cout << "After  growth: data() = " << (void*)marks.data()
              << ", capacity = " << marks.capacity() << '\n';
    std::cout << "If the two data() addresses differ, 'topRef' now dangles.\n";
    std::cout << "Safe fix: store an INDEX (here 1) and re-access marks[1] = "
              << marks[1] << " after growth.\n";
    return 0;
}
