// Q5 - College Bus Seat Tickets (Complete the Code: TODO 1 & TODO 2 filled)
// Student: N Rishitha  |  Register No: 192425011
//
// 'issued' is STATIC: one counter shared by every BusTicket, so the office
// knows how many seats were given without looking at any single ticket.

#include <iostream>
#include <string>

class BusTicket {
    static int issued;               // shared by all BusTicket objects
    static const int CAPACITY = 3;
    int seat;
    std::string name;

public:
    explicit BusTicket(const std::string& n) : seat(0), name(n)
    {
        // TODO 1: If fewer than CAPACITY tickets have been issued, increase
        //         issued and give this ticket the next seat number;
        //         otherwise leave seat as 0 (waiting list).   -> COMPLETED
        if (issued < CAPACITY) {
            ++issued;
            seat = issued;           // next seat number: 1, 2, 3 ...
        }
    }

    static int count()
    {
        // TODO 2: Return the number of seats issued so far.   -> COMPLETED
        return issued;
    }

    void print() const
    {
        std::cout << name << ": "
                  << (seat ? "Seat " + std::to_string(seat) : std::string("Waiting"))
                  << '\n';
    }
};

int BusTicket::issued = 0;           // definition of the static data member

int main()
{
    BusTicket trip[4] = {BusTicket("Asha"), BusTicket("Bala"),
                         BusTicket("Chitra"), BusTicket("Dev")};
    for (const BusTicket& t : trip) t.print();
    std::cout << "Issued: " << BusTicket::count() << '\n';
}
