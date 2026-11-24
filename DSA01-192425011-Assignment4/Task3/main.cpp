// Task 3 - constructors in derived classes, member classes, order trace
#include <iostream>
#include <string>
struct Address {
    std::string city;
    Address(const std::string& c) : city(c) { std::cout << "  Address(" << city << ")\n"; }
    ~Address() { std::cout << "  ~Address\n"; }
};
struct Course {
    std::string code;
    Course(const std::string& c) : code(c) { std::cout << "  Course(" << code << ")\n"; }
    ~Course() { std::cout << "  ~Course\n"; }
};
struct Person {
    std::string name; Address addr;
    Person(const std::string& n, const std::string& city) : name(n), addr(city) { std::cout << "  Person(" << name << ")\n"; }
    virtual ~Person() { std::cout << "  ~Person\n"; }
};
struct Student : virtual Person {
    Course course;
    Student(const std::string& n, const std::string& city, const std::string& c)
        : Person(n, city), course(c) { std::cout << "  Student\n"; }
    ~Student() { std::cout << "  ~Student\n"; }
};
struct Staff : virtual Person {
    double salary;
    Staff(const std::string& n, const std::string& city, double s) : Person(n, city), salary(s) { std::cout << "  Staff\n"; }
    ~Staff() { std::cout << "  ~Staff\n"; }
};
struct TeachingAssistant : Student, Staff {
    // most-derived class constructs the virtual base DIRECTLY; Person(...) in Student/Staff lists is ignored
    TeachingAssistant(const std::string& n, const std::string& city)
        : Person(n, city), Student(n, city, "CS101"), Staff(n, city, 15000) { std::cout << "  TeachingAssistant\n"; }
    ~TeachingAssistant() { std::cout << "  ~TeachingAssistant\n"; }
};
// non-virtual versions for the experiment (sizes only)
struct P2 { std::string name; std::string city; virtual ~P2() = default; };
struct S2 : P2 { std::string course; };
struct F2 : P2 { double salary; };
struct TA2 : S2, F2 {};
int main() {
    std::cout << "== Student ==\n";           { Student s("Asha", "Chennai", "CS201"); }
    std::cout << "== Staff ==\n";             { Staff f("Ravi", "Madurai", 30000); }
    std::cout << "== TeachingAssistant ==\n"; { TeachingAssistant t("Meena", "Salem"); }
    std::cout << "sizeof(TeachingAssistant) virtual = " << sizeof(TeachingAssistant)
              << ", non-virtual (two Person copies) = " << sizeof(TA2) << '\n';
}
