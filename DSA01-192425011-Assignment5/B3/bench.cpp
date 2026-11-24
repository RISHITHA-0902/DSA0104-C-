// B3 - virtual vs switch vs std::variant ; exceptions vs return codes vs optional (compile with -O2)
#include <chrono>
#include <iostream>
#include <memory>
#include <optional>
#include <random>
#include <variant>
#include <vector>
using Clock = std::chrono::steady_clock;
static double ms(Clock::time_point a, Clock::time_point b) { return std::chrono::duration<double, std::milli>(b - a).count(); }

struct VBill { virtual ~VBill() = default; virtual double total() const = 0; };
struct VOP : VBill { double c, t; VOP(double a, double b) : c(a), t(b) {} double total() const override { return c + t; } };
struct VIP : VBill { double d, r, s; VIP(double a, double b, double x) : d(a), r(b), s(x) {} double total() const override { return d * r + s; } };
struct VER : VBill { double b, h, n; VER(double a, double x, double y) : b(a), h(x), n(y) {} double total() const override { return b + h * n; } };

enum Kind { OP, IP, ER };
struct SBill { Kind k; double a, b, c;
    double total() const { switch (k) { case OP: return a + b; case IP: return a * b + c; case ER: return a + b * c; } return 0; } };

struct OPv { double c, t; double total() const { return c + t; } };
struct IPv { double d, r, s; double total() const { return d * r + s; } };
struct ERv { double b, h, n; double total() const { return b + h * n; } };
using VarBill = std::variant<OPv, IPv, ERv>;

struct Data { int k; double a, b, c; };
std::vector<Data> make(size_t n) {
    std::mt19937 g(7); std::uniform_int_distribution<int> kd(0, 2); std::uniform_real_distribution<double> v(1, 1000);
    std::vector<Data> d(n); for (auto& x : d) x = {kd(g), v(g), v(g), v(g)}; return d;
}
void bills(size_t n, bool print) {
    auto d = make(n);
    std::vector<std::unique_ptr<VBill>> vb; std::vector<SBill> sb; std::vector<VarBill> var;
    vb.reserve(n); sb.reserve(n); var.reserve(n);
    for (auto& x : d) {
        if (x.k == 0) { vb.push_back(std::make_unique<VOP>(x.a, x.b)); var.push_back(OPv{x.a, x.b}); }
        if (x.k == 1) { vb.push_back(std::make_unique<VIP>(x.a, x.b, x.c)); var.push_back(IPv{x.a, x.b, x.c}); }
        if (x.k == 2) { vb.push_back(std::make_unique<VER>(x.a, x.b, x.c)); var.push_back(ERv{x.a, x.b, x.c}); }
        sb.push_back({Kind(x.k), x.a, x.b, x.c});
    }
    auto t0 = Clock::now(); double s1 = 0; for (auto& b : vb) s1 += b->total();
    auto t1 = Clock::now(); double s2 = 0; for (auto& b : sb) s2 += b.total();
    auto t2 = Clock::now(); double s3 = 0; for (auto& b : var) s3 += std::visit([](auto& x) { return x.total(); }, b);
    auto t3 = Clock::now();
    if (print) std::cout << "10 bills: virtual=" << s1 << " switch=" << s2 << " variant=" << s3 << (s1 == s2 && s2 == s3 ? "  AGREE\n" : "  DIFFER\n");
    else std::cout << n << " bills: virtual " << ms(t0, t1) << " ms | switch " << ms(t1, t2) << " ms | variant " << ms(t2, t3) << " ms\n";
}
// validation
struct Invalid {};
double vExc(double x) { if (x < 0) throw Invalid{}; return x; }
bool vCode(double x, double& out) { if (x < 0) return false; out = x; return true; }
std::optional<double> vOpt(double x) { if (x < 0) return std::nullopt; return x; }
void validation(int pct) {
    const int n = 1000000; std::vector<double> a(n); std::mt19937 g(1); std::uniform_int_distribution<int> p(0, 99);
    for (auto& x : a) x = p(g) < pct ? -1.0 : 100.0;
    auto t0 = Clock::now(); double s = 0; int bad = 0;
    for (double x : a) { try { s += vExc(x); } catch (const Invalid&) { ++bad; } }
    auto t1 = Clock::now(); double s2 = 0; int bad2 = 0;
    for (double x : a) { double o; if (vCode(x, o)) s2 += o; else ++bad2; }
    auto t2 = Clock::now(); double s3 = 0; int bad3 = 0;
    for (double x : a) { auto o = vOpt(x); if (o) s3 += *o; else ++bad3; }
    auto t3 = Clock::now();
    std::cout << pct << "% invalid: exceptions " << ms(t0, t1) << " ms | return codes " << ms(t1, t2)
              << " ms | optional " << ms(t2, t3) << " ms  (rejected " << bad << "/" << bad2 << "/" << bad3 << ")\n";
}
int main() {
    bills(10, true);
    for (size_t n : {1000UL, 1000000UL, 10000000UL}) bills(n, false);
    std::cout << "sizeof: SBill=" << sizeof(SBill) << " VarBill=" << sizeof(VarBill) << " VOP(+vptr)=" << sizeof(VOP) << " VIP=" << sizeof(VIP) << '\n';
    for (int pct : {0, 1, 50}) validation(pct);
}
