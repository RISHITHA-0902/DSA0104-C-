// Q7 - Complex class for an AC circuit impedance calculator
#include <cmath>
#include <complex>
#include <iomanip>
#include <iostream>
#include <stdexcept>
class Complex {
    double re, im;
public:
    Complex() : re(0), im(0) {}                       // default: 0 + 0j
    Complex(double r) : re(r), im(0) {}               // resistor R + 0j (implicit on purpose: Z + 5)
    Complex(double r, double i) : re(r), im(i) {}
    double real() const { return re; }
    double imag() const { return im; }
    double mag() const { return std::hypot(re, im); }
    Complex& operator+=(const Complex& o) { re += o.re; im += o.im; return *this; }
    friend Complex operator+(Complex a, const Complex& b) { return a += b; }
    friend Complex operator-(const Complex& a, const Complex& b) { return {a.re - b.re, a.im - b.im}; }
    friend Complex operator*(const Complex& a, const Complex& b) { return {a.re * b.re - a.im * b.im, a.re * b.im + a.im * b.re}; }
    friend Complex operator/(const Complex& a, const Complex& b) {
        double den = b.re * b.re + b.im * b.im;
        if (den < 1e-300) throw std::domain_error("division by zero impedance");
        return {(a.re * b.re + a.im * b.im) / den, (a.im * b.re - a.re * b.im) / den};
    }
    friend bool operator==(const Complex& a, const Complex& b) { return std::fabs(a.re - b.re) < 1e-9 && std::fabs(a.im - b.im) < 1e-9; }
    friend std::ostream& operator<<(std::ostream& os, const Complex& z) {
        return os << z.re << (z.im < 0 ? " - j" : " + j") << std::fabs(z.im);
    }
};
Complex parallel(const Complex& a, const Complex& b) { return (a * b) / (a + b); }
int main() {
    const double PI = 3.14159265358979, f = 50, w = 2 * PI * f, R = 100, L = 0.5, C = 20e-6;
    Complex ZR(R), ZL(0, w * L), ZC(0, -1 / (w * C));
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "w = " << w << " rad/s\nZR = " << ZR << "\nZL = " << ZL << "\nZC = " << ZC << '\n';
    Complex series = ZR + ZL + ZC;
    std::cout << "Series   Z = " << series << "  |Z| = " << series.mag() << " ohm\n";
    Complex par = parallel(parallel(ZR, ZL), ZC);
    std::cout << "Parallel Z = " << par << "  |Z| = " << par.mag() << " ohm\n";
    std::complex<double> sR(R), sL(0, w * L), sC(0, -1 / (w * C));
    auto sp = 1.0 / (1.0 / sR + 1.0 / sL + 1.0 / sC);
    std::cout << "std::complex check: series " << sR + sL + sC << " parallel " << sp << '\n';
    std::cout << "(1+2j)*(3-4j) = " << Complex(1, 2) * Complex(3, -4) << "  equal to 11+2j? "
              << std::boolalpha << (Complex(1, 2) * Complex(3, -4) == Complex(11, 2)) << '\n';
    try { std::cout << Complex(5, 5) / Complex(); } catch (const std::exception& e) { std::cout << "Error: " << e.what() << '\n'; }
    std::cout << "std::complex 5/0 = " << std::complex<double>(5, 5) / std::complex<double>(0, 0) << "  (no error, NaN/inf)\n";
}
