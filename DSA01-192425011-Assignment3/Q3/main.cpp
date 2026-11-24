// Q3 - Auditorium seat booking: dynamic constructor + destructor
#include <iostream>
#include <string>

class Event {
    std::string name;
    int rows, seats;
    char** map;          // array of row pointers
    int booked = 0;
public:
    Event(const std::string& n, int r, int s) : name(n), rows(r < 0 ? 0 : r), seats(s < 0 ? 0 : s), map(nullptr) {
        if (rows > 0) {
            map = new char*[rows];
            for (int i = 0; i < rows; ++i) {
                map[i] = new char[seats];
                for (int j = 0; j < seats; ++j) map[i][j] = '.';
            }
        }
        std::cout << "CTOR " << name << " (" << rows << "x" << seats << ", "
                  << bytes() << " bytes)\n";
    }
    Event(const Event&) = delete;              // owns memory: forbid copying
    Event& operator=(const Event&) = delete;
    ~Event() {
        for (int i = 0; i < rows; ++i) delete[] map[i];
        delete[] map;
        std::cout << "DTOR " << name << ": " << booked << "/" << rows * seats << " seats booked, memory released\n";
    }
    long long bytes() const { return (long long)rows * sizeof(char*) + (long long)rows * seats; }
    bool book(int r, int s) {
        if (r < 0 || r >= rows || s < 0 || s >= seats || map[r][s] == 'X') return false;
        map[r][s] = 'X'; ++booked; return true;
    }
    bool cancel(int r, int s) {
        if (r < 0 || r >= rows || s < 0 || s >= seats || map[r][s] != 'X') return false;
        map[r][s] = '.'; --booked; return true;
    }
    void show() const { for (int i = 0; i < rows; ++i) { std::cout << "  "; for (int j = 0; j < seats; ++j) std::cout << map[i][j]; std::cout << '\n'; } }
};

int main() {
    Event stackEv("StackShow", 3, 5);                 // stack object
    stackEv.book(0, 0); stackEv.book(1, 2); stackEv.book(1, 2);
    stackEv.cancel(0, 0); stackEv.book(2, 4); stackEv.show();
    {
        Event blockEv("BlockShow", 2, 3);              // block scope
        blockEv.book(0, 1);
        std::cout << "leaving block\n";
    }
    Event* heapEv = new Event("HeapShow", 4, 4);      // heap object
    heapEv->book(3, 3);
    delete heapEv;
    Event empty("ZeroRows", 0, 10);                    // zero rows
    std::cout << "book on zero rows: " << std::boolalpha << empty.book(0, 0) << '\n';
    for (auto d : {std::pair<int,int>{10,20}, {100,200}, {1000,2000}}) {
        Event e("Size", d.first, d.second);
    }
    std::cout << "end of main\n";
}
