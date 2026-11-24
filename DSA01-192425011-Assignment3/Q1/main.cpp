// Q1 - Library Book Records: constructors, overloading, delegation, explicit
#include <iostream>
#include <stdexcept>
#include <string>

class Book {
    std::string title;
    std::string author;
    int copies;
    static int checked(int c) {
        if (c < 0) throw std::invalid_argument("copies cannot be negative: " + std::to_string(c));
        return c;
    }
public:
    // target constructor: the ONLY place copies is validated
    Book(const std::string& t, const std::string& a, int c)
        : title(t), author(a), copies(checked(c)) { std::cout << "[full ctor] "; }
    // donated book: only a title -> delegates
    explicit Book(const std::string& t) : Book(t, "Unknown", 1) { std::cout << "[title ctor] "; }
    // empty record -> delegates
    Book() : Book("<empty>", "<none>", 0) { std::cout << "[default ctor] "; }

    void print() const { std::cout << "Book{" << title << ", " << author << ", copies=" << copies << "}\n"; }
};

void shelve(const Book& b) { b.print(); }

int main() {
    Book a("Let Us C", "Kanetkar", 5); a.print();
    Book b("Donated Atlas");           b.print();
    Book c;                            c.print();
    Book shelf[2];                     // needs default ctor
    shelf[0].print();
    try { Book bad("X", "Y", -3); }
    catch (const std::exception& e) { std::cout << "\nRejected: " << e.what() << '\n'; }
    // shelve(std::string("Gift"));   // compiles WITHOUT explicit, fails WITH explicit
    shelve(Book("Gift"));             // must be written explicitly
}
