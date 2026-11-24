// B1.4 - virtual destructor removed (run under -fsanitize=address)
#include <iostream>
#include <string>
struct Bill { std::string patient; Bill(const std::string& p) : patient(p) {} ~Bill() { std::cout << "~Bill\n"; } virtual double total() const = 0; };
struct InPatient : Bill {
    std::string ward = "General ward with a long description stored on the heap";
    InPatient() : Bill("Meena") {}
    ~InPatient() { std::cout << "~InPatient\n"; }
    double total() const override { return 47500; }
};
int main() { Bill* b = new InPatient; std::cout << b->total() << '\n'; delete b; }
