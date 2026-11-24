// Copy counting, self-assignment and copy timing
#include <chrono>
#include <iostream>
#include <string>
static int copies = 0;
class Prescription {
    int n; std::string* m;
public:
    explicit Prescription(int c) : n(c), m(new std::string[c]) {}
    Prescription(const Prescription& o) : n(o.n), m(new std::string[o.n]) {
        ++copies; for (int i = 0; i < n; ++i) m[i] = o.m[i];
    }
    Prescription& operator=(const Prescription& o) {
        if (this == &o) { std::cout << "self-assignment ignored\n"; return *this; }
        std::string* f = new std::string[o.n];
        for (int i = 0; i < o.n; ++i) f[i] = o.m[i];
        delete[] m; m = f; n = o.n; return *this;
    }
    ~Prescription() { delete[] m; }
    void set(int i, const std::string& s) { m[i] = s; }
    std::string get(int i) const { return m[i]; }
};
void byValue(Prescription) {}
void byRef(const Prescription&) {}
int main() {
    Prescription p(3);
    copies = 0; byValue(p); std::cout << "copies when passed by value     : " << copies << '\n';
    copies = 0; byRef(p);   std::cout << "copies when passed by const ref : " << copies << '\n';
    p.set(0, "ORS"); p = p; std::cout << "after p = p, p[0] = " << p.get(0) << '\n';
    for (int n : {10, 1000, 100000}) {
        Prescription s(n); for (int i = 0; i < n; ++i) s.set(i, "Medicine-" + std::to_string(i));
        auto t0 = std::chrono::steady_clock::now();
        Prescription c = s;
        auto us = std::chrono::duration<double, std::micro>(std::chrono::steady_clock::now() - t0).count();
        std::cout << "copy of " << n << " medicines: " << us << " us\n";
    }
}
