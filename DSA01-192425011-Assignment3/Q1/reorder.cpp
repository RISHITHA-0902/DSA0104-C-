// Experiment: members initialised in DECLARATION order, not list order (-Wreorder)
#include <iostream>
struct Book {
    int copies;     // declared first -> initialised first
    int reserved;
    Book(int r) : reserved(r), copies(reserved * 2) {}   // copies reads garbage
};
int main() { Book b(4); std::cout << "copies=" << b.copies << " reserved=" << b.reserved << '\n'; }
