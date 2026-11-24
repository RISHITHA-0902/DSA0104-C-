// Q5 - Time (++, ==, <) and Playlist ([] const/non-const with range check)
#include <chrono>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
class Time {
    int h = 0, m = 0, s = 0;
public:
    Time(int hh = 0, int mm = 0, int ss = 0) : h(hh), m(mm), s(ss) {}
    Time& operator++() {                 // prefix: advance, return *this
        if (++s == 60) { s = 0; if (++m == 60) { m = 0; ++h; } }
        return *this;
    }
    Time operator++(int) { Time old = *this; ++*this; return old; }   // postfix: copy
    bool operator==(const Time& o) const { return h == o.h && m == o.m && s == o.s; }
    bool operator<(const Time& o) const { return h != o.h ? h < o.h : m != o.m ? m < o.m : s < o.s; }
    friend std::ostream& operator<<(std::ostream& os, const Time& t) {
        if (t.h) os << t.h << ':' << std::setw(2) << std::setfill('0') << t.m;
        else os << t.m;
        return os << ':' << std::setw(2) << std::setfill('0') << t.s << std::setfill(' ');
    }
};
class Playlist {
    std::string tracks[5]; int n = 0;
public:
    void add(const std::string& t) { if (n < 5) tracks[n++] = t; }
    int size() const { return n; }
    std::string& operator[](int i) {
        if (i < 0 || i >= n) throw std::out_of_range("track index " + std::to_string(i) + " out of range");
        return tracks[i];
    }
    const std::string& operator[](int i) const {
        if (i < 0 || i >= n) throw std::out_of_range("track index " + std::to_string(i) + " out of range");
        return tracks[i];
    }
};
int main() {
    Time t(0, 0, 10), a, b;
    a = t++; b = ++t;
    std::cout << "a = " << a << "  b = " << b << "  t = " << t << '\n';
    Time r(0, 59, 59); ++r; std::cout << "59:59 + 1s -> " << r << '\n';
    std::cout << std::boolalpha << "a < b: " << (a < b) << "  b == t: " << (b == t) << '\n';
    Playlist pl; pl.add("Intro"); pl.add("Melody"); pl.add("Finale");
    pl[1] = "Melody (remix)";                          // non-const []
    const Playlist& cp = pl;
    Time pos; Time trackLen(0, 0, 3);
    for (int i = 0; i < cp.size(); ++i) {              // const [] in playback
        Time end = pos; for (int k = 0; k < 3; ++k) ++end;
        std::cout << "Playing " << cp[i] << " from " << pos << " to " << end << '\n';
        while (pos < end) ++pos;
    }
    for (int bad : {-1, pl.size()})
        try { std::cout << pl[bad]; } catch (const std::exception& e) { std::cout << "Error: " << e.what() << '\n'; }
    // timing 10,000,000 increments
    using C = std::chrono::steady_clock;
    Time x; auto t0 = C::now(); for (int i = 0; i < 10000000; ++i) ++x;
    auto t1 = C::now(); Time y; for (int i = 0; i < 10000000; ++i) y++;
    auto t2 = C::now();
    std::cout << "prefix  : " << std::chrono::duration<double, std::milli>(t1 - t0).count() << " ms (" << x << ")\n"
              << "postfix : " << std::chrono::duration<double, std::milli>(t2 - t1).count() << " ms (" << y << ")\n";
}
