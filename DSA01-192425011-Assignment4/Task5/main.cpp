// Task 5.1 / 5.2 - four ways to reach an object, this pointer, array of pointers
#include <iostream>
#include <string>
class Cart {
    std::string owner; double sum = 0; int count = 0;
public:
    explicit Cart(const std::string& owner) { this->owner = owner; }   // this-> separates member/param
    Cart& add(double price) { sum += price; ++count; return *this; }   // chaining
    double total() const { return sum; }
    const std::string& name() const { return owner; }
    const Cart* self() const { return this; }
};
int main() {
    Cart a("Anu");                 a.add(50).add(30);              // (a) by name
    Cart* p = &a;                  p->add(20);                     // (b) pointer + ->
    Cart& r = a;                   r.add(100);                     // (c) reference
    Cart* h = new Cart("Hari");    h->add(400).add(25);            // (d) new / delete
    std::cout << a.name() << " total " << a.total() << " (this == &a: " << std::boolalpha << (a.self() == &a) << ")\n";
    std::cout << h->name() << " total " << h->total() << '\n';
    delete h;

    Cart* active[4] = { new Cart("C1"), new Cart("C2"), new Cart("C3"), new Cart("C4") };
    active[0]->add(120); active[1]->add(560).add(40); active[2]->add(300); active[3]->add(99);
    Cart** best = active;
    for (Cart** it = active + 1; it != active + 4; ++it)          // pointer arithmetic
        if ((*it)->total() > (*best)->total()) best = it;
    std::cout << "Highest cart: " << (*best)->name() << " = " << (*best)->total() << '\n';
    for (Cart* c : active) delete c;
}
