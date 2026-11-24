// B4.1 - new bill type and new payer WITHOUT changing billing.hpp ; B4.2 run under ASan
#include "../B1/billing.hpp"
#include <memory>
#include <vector>
class DialysisBill : public Bill {                 // NEW bill type
    int sessions; double perSession;
public:
    DialysisBill(const std::string& p, int s, double r) : Bill(p), sessions((int)nonNegative(s, "sessions")), perSession(nonNegative(r, "session rate")) {}
    double total() const override { return sessions * perSession; }
    std::string type() const override { return "DL"; }
    std::string package() const override { return "DIALYSIS"; }
};
class CorporateTieUp : public Payer {             // NEW payer: company pays 80%, max 25000 per bill
public:
    double amountCovered(const Bill& b) const override { return std::min(b.total() * 0.8, 25000.0); }
    std::string name() const override { return "Corporate"; }
};
int main() {
    std::vector<std::unique_ptr<Bill>> bills;
    bills.push_back(std::make_unique<OutPatientBill>("Ravi", 500, 1200));
    bills.push_back(std::make_unique<InPatientBill>("Meena", 3, 2500, 40000, 6000));
    bills.push_back(std::make_unique<DayCareBill>("Arjun", "CATARACT", 18000));
    bills.push_back(std::make_unique<EmergencyBill>("Lakshmi", 3000, 1500, 4));
    bills.push_back(std::make_unique<DialysisBill>("Kannan", 8, 2200));
    SelfPay self; Insurance ins(50000, 10); GovernmentScheme gov; CorporateTieUp corp;
    const Payer* payers[] = {&self, &ins, &gov, &corp};
    const double expected[] = {1700, 53500, 18000, 9000, 17600};
    int pass = 0, i = 0;
    for (const auto& b : bills) {
        bool ok = b->total() == expected[i++]; pass += ok;
        std::cout << (ok ? "PASS " : "FAIL ") << b->type() << ' ' << b->name() << " total " << b->total() << " |";
        for (auto* p : payers) {
            try { double c = p->amountCovered(*b); std::cout << ' ' << p->name() << '=' << c; }
            catch (const ClaimError& e) { std::cout << ' ' << p->name() << "=REJECTED(" << e.what() << ')'; }
        }
        std::cout << '\n';
    }
    std::cout << pass << "/5 totals correct\n";
    try { DialysisBill bad("X", -2, 2200); } catch (const BillingError& e) { std::cout << "caught: " << e.what() << '\n'; }
}
