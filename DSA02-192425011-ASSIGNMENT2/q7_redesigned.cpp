// Q7 - REDESIGNED: circular buffer (ring) with a start index. O(1) per add.
// Student: N Rishitha  |  Register No: 192425011
//
// Objective: make add() cost independent of the window size KEEP.
// Change: instead of shifting, keep a 'head' index and overwrite the oldest
// slot in place; advance head modulo KEEP. Same memory (one fixed array),
// no extra allocation. Trade-off: reading oldest->newest needs modular
// indexing, but that is O(KEEP) only when you actually print, not per add.

#include <iostream>
#include <chrono>

class HistoryRing {
    static const int KEEP = 2048;    // same window as the original
    long long buf[KEEP];
    int count = 0;
    int head = 0;                    // next write slot
public:
    void add(long long v)
    {
        buf[head] = v;                       // overwrite oldest - O(1)
        head = (head + 1) % KEEP;
        if (count < KEEP) ++count;
    }
    long long newest() const { return count ? buf[(head - 1 + KEEP) % KEEP] : 0; }
    long long oldest() const
    {
        if (!count) return 0;
        int start = (head - count % KEEP + KEEP) % KEEP;
        return buf[start];
    }
};

int main()
{
    const long long M = 1'000'000;      // same workload as the original
    HistoryRing h;

    auto t0 = std::chrono::high_resolution_clock::now();
    for (long long i = 1; i <= M; ++i) h.add(i);
    auto t1 = std::chrono::high_resolution_clock::now();

    double ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
    std::cout << "REDESIGN (circular buf): " << M << " adds in " << ms << " ms\n";
    std::cout << "  oldest kept = " << h.oldest()
              << ", newest = " << h.newest() << '\n';
    return 0;
}
