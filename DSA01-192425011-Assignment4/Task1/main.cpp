// Task 1.2 - Repaired payroll hierarchy
#include <iostream>
#include <memory>
#include <string>
#include <vector>

class Staff {                                   // abstract base
protected:
    std::string name;
    int id;
public:
    Staff(const std::string& n, int i) : name(n), id(i) {}
    virtual ~Staff() = default;
    virtual double pay() const = 0;
    virtual std::string role() const = 0;
    void print() const { std::cout << role() << " #" << id << " " << name << " : " << pay() << '\n'; }
};
class SalariedStaff : public Staff {            // common basic + DA rule written once
protected:
    double basic;
public:
    static constexpr double DA_RATE = 0.42;     // named constant, no magic number
    SalariedStaff(const std::string& n, int i, double b) : Staff(n, i), basic(b) {}
    double pay() const override { return basic + basic * DA_RATE; }
};
class Professor : public SalariedStaff {
    double research;
public:
    Professor(const std::string& n, int i, double b, double r) : SalariedStaff(n, i, b), research(r) {}
    double pay() const override { return SalariedStaff::pay() + research; }
    std::string role() const override { return "Professor"; }
};
class LabAssistant : public SalariedStaff {
public:
    using SalariedStaff::SalariedStaff;
    std::string role() const override { return "LabAssistant"; }
};
class GuestFaculty : public Staff {
    double hours, rate;
public:
    GuestFaculty(const std::string& n, int i, double h, double r) : Staff(n, i), hours(h), rate(r) {}
    double pay() const override { return hours * rate; }
    std::string role() const override { return "GuestFaculty"; }
};
// NEW category added without touching any class above
class Intern : public Staff {
    double stipend;
public:
    Intern(const std::string& n, int i, double s) : Staff(n, i), stipend(s) {}
    double pay() const override { return stipend; }
    std::string role() const override { return "Intern"; }
};

int main() {
    std::vector<std::unique_ptr<Staff>> all;
    all.push_back(std::make_unique<Professor>("Dr. Rao", 101, 80000, 15000));
    all.push_back(std::make_unique<LabAssistant>("Kumar", 202, 25000));
    all.push_back(std::make_unique<GuestFaculty>("Ms. Devi", 303, 40, 1200));
    all.push_back(std::make_unique<Intern>("Arun", 404, 10000));
    double total = 0;
    for (const auto& s : all) { s->print(); total += s->pay(); }   // ONE loop for all staff
    std::cout << "Total payroll: " << total << '\n';
}
