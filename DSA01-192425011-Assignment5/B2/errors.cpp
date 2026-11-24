// B2 - engineered error conditions
#include "../B1/billing.hpp"
#include <memory>
#include <new>
#include <vector>
struct Tracked {                          // proves objects are destroyed during unwinding
    std::string n; explicit Tracked(std::string s) : n(std::move(s)) { std::cout << "  +" << n << '\n'; }
    ~Tracked() { std::cout << "  -" << n << '\n'; }
};
void test(const char* title, void (*fn)()) {
    std::cout << title << '\n';
    try { fn(); std::cout << "  ok\n"; }
    catch (const ClaimError& e)        { std::cout << "  ClaimError: " << e.what() << '\n'; }
    catch (const BillingError& e)      { std::cout << "  BillingError: " << e.what() << '\n'; }
    catch (const std::bad_alloc& e)    { std::cout << "  bad_alloc: " << e.what() << '\n'; }
    catch (const std::invalid_argument& e) { std::cout << "  invalid_argument: " << e.what() << '\n'; }
    catch (...)                        { std::cout << "  unknown error - program continues\n"; }
}
int main() {
    test("1. negative charges", [] { Tracked t("bill-guard"); OutPatientBill b("Ravi", -500, 100); });
    test("2. claim above cover", [] { Tracked t("claim-guard"); InPatientBill b("Meena", 10, 5000, 90000, 0); Insurance(50000, 10).amountCovered(b); });
    test("3. unknown package", [] { DayCareBill b("Arjun", "LASIK", 30000); GovernmentScheme().amountCovered(b); });
    test("4. missing patient record", [] { EmergencyBill b("", 3000, 1000, 2); });
    test("5. text where a number is expected", [] { Tracked t("parser"); double v = std::stod("five hundred"); (void)v; });
    test("6. memory allocation failure", [] { Tracked t("buffer-owner"); volatile size_t huge = 1ULL << 50; double* big = new double[huge]; delete[] big; });
    test("7. non-standard exception (catch(...))", [] { Tracked t("legacy"); throw 404; });
    test("8. valid bill", [] { OutPatientBill b("Ravi", 500, 1200); std::cout << "  total " << b.total() << '\n'; });
    std::cout << "program still running after all errors\n";
}
