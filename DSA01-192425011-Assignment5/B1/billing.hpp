// Hospital billing: polymorphic bills, payers and exception hierarchy (used by B1 and B4)
#pragma once
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>

// ---------- exceptions ----------
struct BillingError : std::runtime_error { using std::runtime_error::runtime_error; };
struct InvalidCharge : BillingError { using BillingError::BillingError; };
struct ClaimError : BillingError { using BillingError::BillingError; };
struct UnknownPackage : ClaimError { using ClaimError::ClaimError; };
struct MissingRecord : BillingError { using BillingError::BillingError; };

inline double nonNegative(double v, const char* what) {
    if (v < 0) throw InvalidCharge(std::string("negative ") + what);
    return v;
}

// ---------- bills ----------
class Bill {
protected:
    std::string patient;
public:
    explicit Bill(const std::string& p) : patient(p) { if (p.empty()) throw MissingRecord("missing patient record"); }
    virtual ~Bill() = default;
    virtual double total() const = 0;
    virtual std::string type() const = 0;
    virtual std::string package() const { return ""; }      // for government schemes
    const std::string& name() const noexcept { return patient; }
    void print() const { std::cout << type() << ' ' << patient << ": " << total() << '\n'; }
};
class OutPatientBill : public Bill {
    double consultation, tests;
public:
    OutPatientBill(const std::string& p, double c, double t)
        : Bill(p), consultation(nonNegative(c, "consultation")), tests(nonNegative(t, "tests")) {}
    double total() const override { return consultation + tests; }
    std::string type() const override { return "OP"; }
    std::string package() const override { return "OPD"; }
};
class InPatientBill : public Bill {
    int days; double perDay, procedures, medicines;
public:
    InPatientBill(const std::string& p, int d, double rate, double proc, double med)
        : Bill(p), days((int)nonNegative(d, "days")), perDay(nonNegative(rate, "room rate")),
          procedures(nonNegative(proc, "procedures")), medicines(nonNegative(med, "medicines")) {}
    double total() const override { return days * perDay + procedures + medicines; }
    std::string type() const override { return "IP"; }
    std::string package() const override { return "IPD"; }
};
class DayCareBill : public Bill {
    std::string pkg; double price;
public:
    DayCareBill(const std::string& p, const std::string& k, double pr) : Bill(p), pkg(k), price(nonNegative(pr, "package price")) {}
    double total() const override { return price; }
    std::string type() const override { return "DC"; }
    std::string package() const override { return pkg; }
};
class EmergencyBill : public Bill {
    double base, perHour; int hours;
public:
    EmergencyBill(const std::string& p, double b, double ph, int h)
        : Bill(p), base(nonNegative(b, "base charge")), perHour(nonNegative(ph, "hourly charge")), hours((int)nonNegative(h, "hours")) {}
    double total() const override { return base + perHour * hours; }
    std::string type() const override { return "ER"; }
    std::string package() const override { return "EMERGENCY"; }
};

// ---------- payers ----------
class Payer {
public:
    virtual ~Payer() = default;
    virtual double amountCovered(const Bill& b) const = 0;
    virtual std::string name() const = 0;
    double patientPays(const Bill& b) const { return b.total() - amountCovered(b); }
};
class SelfPay : public Payer {
public:
    double amountCovered(const Bill&) const override { return 0; }
    std::string name() const override { return "SelfPay"; }
};
class Insurance : public Payer {
    double coverLimit, coPayPct;
public:
    Insurance(double limit, double copay) : coverLimit(nonNegative(limit, "cover")), coPayPct(copay) {}
    double amountCovered(const Bill& b) const override {
        double claim = b.total() * (100 - coPayPct) / 100;
        if (claim > coverLimit) throw ClaimError("claim " + std::to_string((long)claim) + " exceeds cover " + std::to_string((long)coverLimit));
        return claim;
    }
    std::string name() const override { return "Insurance"; }
};
class GovernmentScheme : public Payer {
    std::map<std::string, double> rates;     // fixed package rates
public:
    GovernmentScheme() : rates{{"OPD", 1000}, {"IPD", 50000}, {"CATARACT", 15000}, {"EMERGENCY", 8000}} {}
    double amountCovered(const Bill& b) const override {
        auto it = rates.find(b.package());
        if (it == rates.end()) throw UnknownPackage("unknown package '" + b.package() + "'");
        return std::min(it->second, b.total());
    }
    std::string name() const override { return "GovtScheme"; }
};
