// Q4 - Savings Account Mini-Statement
// Student: N Rishitha  |  Register No: 192425011
//
// Each account remembers its LAST 10 transactions for an ATM mini-statement.
// Older transactions are discarded. The ring (circular) index keeps the add
// cost at O(1) - no shifting.
//
// Some member functions are defined INSIDE the class, some OUTSIDE using the
// scope-resolution operator (::).

#include <iostream>
#include <string>

class SavingsAccount {
    // ---- private data: the array MUST be private so no outside code can
    //      corrupt the history or the count ------------------------------
    long long accountNo;
    long long balance;
    static const int KEEP = 10;      // how many transactions we remember
    long long history[KEEP];         // fixed ring buffer of last KEEP txns
    int count;                       // how many real txns recorded (caps at KEEP)
    int head;                        // ring index of the next write slot

public:
    // defined INSIDE the class
    SavingsAccount(long long acc, long long opening)
        : accountNo(acc), balance(opening), count(0), head(0) {}

    long long getBalance() const { return balance; }

    // declared here, DEFINED OUTSIDE the class (see below)
    void deposit(long long amount);
    bool withdraw(long long amount);
    void miniStatement() const;

private:
    void record(long long signedAmount);   // helper, defined outside
};

// ---- member functions defined OUTSIDE using  ClassName::  -----------------

void SavingsAccount::record(long long signedAmount)
{
    history[head] = signedAmount;        // overwrite oldest in O(1)
    head = (head + 1) % KEEP;            // advance ring index
    if (count < KEEP) ++count;
}

void SavingsAccount::deposit(long long amount)
{
    balance += amount;
    record(+amount);
}

bool SavingsAccount::withdraw(long long amount)
{
    if (amount > balance) return false;  // insufficient funds
    balance -= amount;
    record(-amount);
    return true;
}

void SavingsAccount::miniStatement() const
{
    std::cout << "Mini-statement for A/C " << accountNo
              << "  (last " << count << " of max " << KEEP << ")\n";
    // Print oldest -> newest. Oldest sits 'count' steps behind head.
    int start = (head - count % KEEP + KEEP) % KEEP;
    for (int i = 0; i < count; ++i) {
        long long v = history[(start + i) % KEEP];
        std::cout << "  " << (v >= 0 ? "CR " : "DR ")
                  << (v >= 0 ? v : -v) << '\n';
    }
    std::cout << "  Balance: " << balance << '\n';
}

int main()
{
    SavingsAccount acc(192425011, 1000);

    // 12 transactions -> only the last 10 survive in the mini-statement
    for (int i = 1; i <= 7; ++i) acc.deposit(i * 100);   // +100..+700
    acc.withdraw(250);
    acc.withdraw(400);
    acc.deposit(50);
    acc.deposit(60);
    acc.deposit(70);        // this is the 12th transaction

    acc.miniStatement();
    return 0;
}
