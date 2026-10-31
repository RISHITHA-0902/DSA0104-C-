// Q6 - Inline timing benchmark: 10,000,000 calls, inline vs non-inline.
// Student: N Rishitha  |  Register No: 192425011
//
// Build the SAME file four ways and compare the printed times:
//   g++ -O0            q6_timing.cpp -o q6_timing_noinline_O0
//   g++ -O0 -DUSE_INLINE q6_timing.cpp -o q6_timing_inline_O0
//   g++ -O2            q6_timing.cpp -o q6_timing_noinline_O2
//   g++ -O2 -DUSE_INLINE q6_timing.cpp -o q6_timing_inline_O2
//
// USE_INLINE  -> function marked 'inline' (compiler may inline it).
// (default)   -> function marked __attribute__((noinline)) (never inlined),
//                so the per-call overhead is visible.

#include <iostream>
#include <chrono>

class Time {
    int h, m;
public:
    Time(int hh, int mm) : h(hh), m(mm) {}
    friend int minutesBetween(const Time& a, const Time& b);
};

#ifdef USE_INLINE
inline int minutesBetween(const Time& a, const Time& b)
#else
__attribute__((noinline)) int minutesBetween(const Time& a, const Time& b)
#endif
{
    int d = (a.h * 60 + a.m) - (b.h * 60 + b.m);
    return d < 0 ? -d : d;
}

int main()
{
    const long long N = 10'000'000;
    Time b(13, 10);
    volatile long long sink = 0;   // volatile stops the loop being optimised away

    auto t0 = std::chrono::high_resolution_clock::now();
    for (long long i = 0; i < N; ++i) {
        // vary the input each call so the result is NOT loop-invariant;
        // this measures real per-call cost rather than a hoisted constant.
        Time a(static_cast<int>(i % 24), static_cast<int>(i % 60));
        sink += minutesBetween(a, b);
    }
    auto t1 = std::chrono::high_resolution_clock::now();

    double ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
    std::cout <<
#ifdef USE_INLINE
        "inline   "
#else
        "noinline "
#endif
              << N << " calls: " << ms << " ms  (sink=" << sink << ")\n";
    return 0;
}
