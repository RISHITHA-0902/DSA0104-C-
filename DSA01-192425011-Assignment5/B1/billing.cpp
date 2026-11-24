// B1.1 - one bill of each kind with each payer
#include "billing.hpp"
#include <iomanip>
#include <memory>
#include <vector>
int main() {
    std::vector<std::unique_ptr<Bill>> bills;
    bills.push_back(std::make_unique<OutPatientBill>("Ravi", 500, 1200));              // 1700
    bills.push_back(std::make_unique<InPatientBill>("Meena", 3, 2500, 40000, 6000));   // 53500
    bills.push_back(std::make_unique<DayCareBill>("Arjun", "CATARACT", 18000));        // 18000
    bills.push_back(std::make_unique<EmergencyBill>("Lakshmi", 3000, 1500, 4));        // 9000
    SelfPay self; Insurance ins(50000, 10); GovernmentScheme gov;
    const Payer* payers[] = {&self, &ins, &gov};
    double day = 0;
    std::cout << std::left << std::setw(10) << "Bill" << std::setw(9) << "Total";
    for (auto* p : payers) std::cout << std::setw(24) << p->name() + " covers/pays";
    std::cout << '\n';
    for (const auto& b : bills) {
        day += b->total();
        std::cout << std::setw(10) << (b->type() + " " + b->name().substr(0, 6)) << std::setw(9) << b->total();
        for (auto* p : payers) {
            try { std::cout << std::setw(24) << (std::to_string((long)p->amountCovered(*b)) + " / " + std::to_string((long)p->patientPays(*b))); }
            catch (const ClaimError& e) { std::cout << std::setw(24) << ("REJECTED"); }
        }
        std::cout << '\n';
    }
    std::cout << "Day total: " << day << '\n';
}
