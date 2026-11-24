// Same program with 'virtual' removed from ~Bill (run under -fsanitize=address)
#include <iostream>
#include <string>
struct Bill {
    ~Bill() { std::cout << "~Bill "; }
    virtual double total() const = 0;
    void show() const { std::cout << total() << ' '; }
};
struct OPBill : Bill {
    std::string patient = "a patient name long enough to need heap memory";   // makes the leak visible
    ~OPBill() { std::cout << "~OPBill "; }
    double total() const override { return 500; }
};
int main()
{
    Bill* b = new OPBill;
    b->show();
    delete b;
    std::cout << '\n';
}
