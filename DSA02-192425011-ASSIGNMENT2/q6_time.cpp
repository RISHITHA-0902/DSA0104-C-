// Q6 - Train Journey Planner (Complete the Code: TODO 1 & TODO 2 filled)
// Student: N Rishitha  |  Register No: 192425011
//
// minutesBetween() and earlier() work on TWO Time objects equally, so they
// are FRIEND functions: called like free functions (not a.f(b)), yet allowed
// to read the private members h and m of both arguments.

#include <iostream>

class Time {
    int h, m;
public:
    Time(int hh, int mm) : h(hh), m(mm) {}
    friend int minutesBetween(const Time& a, const Time& b);
    friend bool earlier(const Time& a, const Time& b);
};

int minutesBetween(const Time& a, const Time& b)
{
    // TODO 1: Using the private members h and m, return the absolute
    //         difference between the two times in minutes.   -> COMPLETED
    int ma = a.h * 60 + a.m;
    int mb = b.h * 60 + b.m;
    int diff = ma - mb;
    return diff < 0 ? -diff : diff;
}

bool earlier(const Time& a, const Time& b)
{
    // TODO 2: Return true if a is earlier in the day than b.   -> COMPLETED
    return (a.h * 60 + a.m) < (b.h * 60 + b.m);
}

int main()
{
    Time departure(9, 45), arrival(13, 10);
    std::cout << minutesBetween(departure, arrival) << ' '
              << std::boolalpha << earlier(arrival, departure) << '\n';

    // Extra correctness checks (either side of noon, and same time twice):
    Time noonMinus(11, 59), noonPlus(12, 1), same(8, 0);
    std::cout << "across noon: " << minutesBetween(noonMinus, noonPlus) << ' '
              << earlier(noonMinus, noonPlus) << '\n';
    std::cout << "same twice : " << minutesBetween(same, same) << ' '
              << earlier(same, same) << '\n';
}
