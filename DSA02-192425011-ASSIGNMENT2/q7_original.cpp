// Q7 - ORIGINAL design (the limitation): SHIFTING a fixed array.
// Student: N Rishitha  |  Register No: 192425011
//
// Diagnosed limitation: to keep the last KEEP transactions, every new
// transaction shifts all KEEP-1 older ones down by one slot -> O(KEEP) per
// add. As the window KEEP grows (bank now wants the last 50, or more) and as
// the number of transactions M grows, total work is O(M * KEEP) and gets slow.

#include <iostream>
#include <chrono>

class HistoryShift {
    static const int KEEP = 2048;    // large window to expose the cost
    long long buf[KEEP];
    int count = 0;
public:
    void add(long long v)
    {
        // shift everything down one slot (drop the oldest), then store at end
        int n = (count < KEEP) ? count : KEEP - 1;
        for (int i = 0; i < n; ++i) buf[i] = buf[i + 1];   // O(KEEP) work
        buf[n] = v;
        if (count < KEEP) ++count;
    }
    long long newest() const { return count ? buf[count - 1] : 0; }
    long long oldest() const { return count ? buf[0] : 0; }
};

int main()
{
    const long long M = 1'000'000;      // transactions processed
    HistoryShift h;

    auto t0 = std::chrono::high_resolution_clock::now();
    for (long long i = 1; i <= M; ++i) h.add(i);
    auto t1 = std::chrono::high_resolution_clock::now();

    double ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
    std::cout << "ORIGINAL (shift array): " << M << " adds in " << ms << " ms\n";
    std::cout << "  oldest kept = " << h.oldest()
              << ", newest = " << h.newest() << '\n';
    return 0;
}
