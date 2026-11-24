// Task 2.3 - University class declarations + composition vs virtual inheritance
#include <iostream>
#include <string>
class Person {
protected:
    std::string name; int id; std::string phone;
public:
    Person(const std::string& n, int i, const std::string& p) : name(n), id(i), phone(p) {}
    virtual ~Person() = default;
    const std::string& getName() const { return name; }
};
class Student : public virtual Person {             // is-a, virtual: shared Person
protected:
    std::string programme; double cgpa;
public:
    Student(const std::string& n, int i, const std::string& p, const std::string& prog, double g)
        : Person(n, i, p), programme(prog), cgpa(g) {}
};
class Staff : public virtual Person {
protected:
    std::string department; double salary;
public:
    Staff(const std::string& n, int i, const std::string& p, const std::string& d, double s)
        : Person(n, i, p), department(d), salary(s) {}
};
class TeachingAssistant : public Student, public Staff {   // multiple -> hybrid (diamond)
public:
    TeachingAssistant(const std::string& n, int i, const std::string& p)
        : Person(n, i, p), Student(n, i, p, "M.Tech", 8.5), Staff(n, i, p, "CSE", 20000) {}
};
class Fellowship { std::string agency; double amount; public:
    Fellowship(const std::string& a, double m) : agency(a), amount(m) {} };
class ResearchScholar : public Student {            // is-a Student, has-a Fellowship
    Fellowship fellowship;
public:
    ResearchScholar(const std::string& n, int i, const std::string& p)
        : Person(n, i, p), Student(n, i, p, "PhD", 9.0), fellowship("DST", 37000) {}
};
// Alternative: composition (has-a) - TA contains a Student and a Staff object
class TA_Composed {
    Student asStudent; Staff asStaff;
public:
    TA_Composed(const std::string& n, int i, const std::string& p)
        : asStudent(n, i, p, "M.Tech", 8.5), asStaff(n, i, p, "CSE", 20000) {}
    const std::string& name() const { return asStudent.getName(); }   // must choose a copy explicitly
};
int main() {
    TeachingAssistant ta("Priya", 7, "98400");
    TA_Composed tc("Priya", 7, "98400");
    ResearchScholar rs("Vikram", 9, "90000");
    std::cout << "TA name (one Person, no ambiguity): " << ta.getName() << '\n';
    std::cout << "Composed TA name (chosen copy)     : " << tc.name() << '\n';
    std::cout << "sizeof(Person)=" << sizeof(Person) << " sizeof(TeachingAssistant, virtual)=" << sizeof(TeachingAssistant)
              << " sizeof(TA_Composed)=" << sizeof(TA_Composed) << " sizeof(ResearchScholar)=" << sizeof(ResearchScholar) << '\n';
}
