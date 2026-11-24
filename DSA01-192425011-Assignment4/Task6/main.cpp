// Task 6 - Vehicle rental fleet
#include <iomanip>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

struct Date { int d, m, y; std::string str() const { return std::to_string(d) + "/" + std::to_string(m) + "/" + std::to_string(y); } };

class Vehicle {                                      // abstract base
protected:
    std::string reg; double dailyRate; bool rented = false;
public:
    Vehicle(const std::string& r, double rate) : reg(r), dailyRate(rate) {}
    virtual ~Vehicle() = default;
    virtual double charge(int days) const = 0;       // differs per vehicle
    virtual std::string kind() const = 0;
    virtual double lateFeePerDay() const { return dailyRate * 0.5; }
    bool isRented() const { return rented; }
    const std::string& regNo() const { return reg; }
    void rent() { if (rented) throw std::logic_error(reg + " already rented"); rented = true; }
    double giveBack(int days, int lateDays) {
        if (!rented) throw std::logic_error(reg + " was not rented");
        rented = false;
        return charge(days) + lateDays * lateFeePerDay();
    }
};
class Bicycle : public Vehicle {
public:
    using Vehicle::Vehicle;
    double charge(int days) const override { return days * dailyRate; }
    std::string kind() const override { return "Bicycle"; }
};
class MotorVehicle : public Vehicle {               // abstract intermediate class
protected:
    double level;      // fuel litres or charge kWh to refill
    Date serviceDue;   // member class
public:
    MotorVehicle(const std::string& r, double rate, double lvl, Date due) : Vehicle(r, rate), level(lvl), serviceDue(due) {}
    virtual double energyCost() const = 0;           // per litre or per kWh
    std::string service() const { return serviceDue.str(); }
};
class TwoWheeler : public MotorVehicle {
public:
    using MotorVehicle::MotorVehicle;
    double energyCost() const override { return 105.0; }             // per litre
    double charge(int days) const override { return days * dailyRate + level * energyCost(); }
    std::string kind() const override { return "TwoWheeler"; }
};
class Car : public MotorVehicle {
protected:
    int seats;
public:
    Car(const std::string& r, double rate, double lvl, Date due, int s) : MotorVehicle(r, rate, lvl, due), seats(s) {}
    double energyCost() const override { return 102.0; }
    double charge(int days) const override { return days * dailyRate + level * energyCost() + (seats > 5 ? 500 * days : 0); }
    std::string kind() const override { return "Car"; }
};
class ElectricCar : public Car {                    // multilevel
    double batteryKWh;
public:
    ElectricCar(const std::string& r, double rate, double lvl, Date due, int s, double b) : Car(r, rate, lvl, due, s), batteryKWh(b) {}
    double energyCost() const override { return 12.0; }              // per kWh
    std::string kind() const override { return "ElectricCar(" + std::to_string((int)batteryKWh) + "kWh)"; }
};
// ---- extension: added later WITHOUT changing any class above ----
class ElectricBus : public MotorVehicle {
    int seats;
public:
    ElectricBus(const std::string& r, double rate, double lvl, Date due, int s) : MotorVehicle(r, rate, lvl, due), seats(s) {}
    double energyCost() const override { return 12.0; }
    double charge(int days) const override { return days * dailyRate + level * energyCost() + seats * 10.0 * days; }
    double lateFeePerDay() const override { return dailyRate; }
    std::string kind() const override { return "ElectricBus"; }
};

class Fleet {
    std::vector<std::unique_ptr<Vehicle>> v;         // base-class pointers
public:
    void add(std::unique_ptr<Vehicle> x) { v.push_back(std::move(x)); }
    Vehicle* find(const std::string& r) { for (auto& x : v) if (x->regNo() == r) return x.get(); return nullptr; }
    void list() const {
        for (auto& x : v) std::cout << "  " << std::left << std::setw(20) << x->kind() << x->regNo()
                                    << (x->isRented() ? "  RENTED" : "  available") << '\n';
    }
};
int main() {
    Fleet f; Date due{15, 1, 2027};
    f.add(std::make_unique<Bicycle>("BC-01", 100));
    f.add(std::make_unique<TwoWheeler>("TN01-TW-11", 400, 2, due));
    f.add(std::make_unique<Car>("TN09-CR-22", 2000, 10, due, 7));
    f.add(std::make_unique<ElectricCar>("TN10-EV-33", 2500, 30, due, 5, 40));
    f.add(std::make_unique<ElectricBus>("TN11-EB-44", 8000, 150, due, 40));
    std::cout << "Fleet:\n"; f.list();
    for (const char* r : {"BC-01", "TN01-TW-11", "TN09-CR-22", "TN10-EV-33", "TN11-EB-44"}) f.find(r)->rent();
    std::cout << "After renting all:\n"; f.list();
    try { f.find("BC-01")->rent(); } catch (const std::exception& e) { std::cout << "Error: " << e.what() << '\n'; }
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Return BC-01 3 days            : " << f.find("BC-01")->giveBack(3, 0) << '\n';
    std::cout << "Return TN01-TW-11 2 days       : " << f.find("TN01-TW-11")->giveBack(2, 0) << '\n';
    std::cout << "Return TN09-CR-22 2 days +1 late: " << f.find("TN09-CR-22")->giveBack(2, 1) << '\n';
    std::cout << "Return TN10-EV-33 1 day        : " << f.find("TN10-EV-33")->giveBack(1, 0) << '\n';
    std::cout << "Return TN11-EB-44 1 day +2 late: " << f.find("TN11-EB-44")->giveBack(1, 2) << '\n';
    std::cout << "Final:\n"; f.list();
}
