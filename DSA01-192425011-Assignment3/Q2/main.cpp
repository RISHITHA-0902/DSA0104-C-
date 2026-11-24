// Q2 - Complete the Code: deep copy (Rule of Three)
#include <iostream>
#include <string>

class Prescription {
    int n;
    std::string* medicines;        // dynamically allocated array
public:
    explicit Prescription(int count) : n(count), medicines(new std::string[count]) {}
    // TODO 1
    Prescription(const Prescription& o) : n(o.n), medicines(new std::string[o.n]) {
        for (int i = 0; i < n; ++i) medicines[i] = o.medicines[i];
    }
    // TODO 2
    Prescription& operator=(const Prescription& o) {
        if (this == &o) return *this;
        std::string* fresh = new std::string[o.n];
        for (int i = 0; i < o.n; ++i) fresh[i] = o.medicines[i];
        delete[] medicines;
        medicines = fresh; n = o.n;
        return *this;
    }
    ~Prescription() {
        delete[] medicines;   // TODO 3
    }
    void set(int i, const std::string& m) { medicines[i] = m; }
    std::string get(int i) const { return medicines[i]; }
};

int main()
{
    Prescription p(2);
    p.set(0, "Paracetamol");
    p.set(1, "ORS");
    Prescription q = p;           // copy constructor
    q.set(1, "Cetirizine");
    Prescription r(1);
    r = p;                    // copy assignment
    p.set(0, "Ibuprofen");
    std::cout << p.get(0) << ' ' << p.get(1) << " | " << q.get(0) << ' ' << q.get(1)
              << " | " << r.get(0) << ' ' << r.get(1) << '\n';
}
