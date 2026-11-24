// Task 4.2 - original defective program with destructor messages (run with -fsanitize=address)
#include <iostream>
class Account { public: double rate() const { return 3.5; } ~Account() { std::cout << "~Account\n"; } };
class SeniorAccount : public Account {
    double* history = new double[100];
public: double rate() const { return 4.0; } ~SeniorAccount() { std::cout << "~SeniorAccount\n"; delete[] history; } };
int main() {
    Account* accounts[2] = { new Account, new SeniorAccount };
    for (Account* a : accounts) std::cout << a->rate() << "\n";   // 3.5 3.5 : static binding
    for (Account* a : accounts) delete a;                         // ~SeniorAccount never runs
    Account copy = SeniorAccount();                               // slicing
    std::cout << "copy rate: " << copy.rate() << '\n';
}
