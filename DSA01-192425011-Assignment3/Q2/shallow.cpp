// Shallow version: no copy constructor / assignment -> shared array, double delete
#include <iostream>
#include <string>
class Prescription {
    int n; std::string* medicines;
public:
    explicit Prescription(int c) : n(c), medicines(new std::string[c]) {}
    ~Prescription() { delete[] medicines; }
    void set(int i, const std::string& m) { medicines[i] = m; }
    std::string get(int i) const { return medicines[i]; }
};
int main() {
    Prescription p(2); p.set(0, "Paracetamol"); p.set(1, "ORS");
    Prescription q = p;            // copies the POINTER only
    q.set(1, "Cetirizine");
    std::cout << "p[1] = " << p.get(1) << "  (original changed!)\n";
}   // q and p both delete[] the same array
