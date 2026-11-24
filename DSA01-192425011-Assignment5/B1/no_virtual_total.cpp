// B1.3 - total() not virtual (base gives 0): calls through Bill* use Bill::total
#include <iostream>
struct Bill { virtual ~Bill() = default; double total() const { return 0; } };
struct OutPatient : Bill { double total() const { return 1700; } };
struct InPatient : Bill { double total() const { return 47500; } };
int main() {
    Bill* bills[] = {new OutPatient, new InPatient};
    double sum = 0;
    for (Bill* b : bills) { std::cout << b->total() << '\n'; sum += b->total(); }
    std::cout << "Total " << sum << "  (expected 49200)\n";
    for (Bill* b : bills) delete b;
}
