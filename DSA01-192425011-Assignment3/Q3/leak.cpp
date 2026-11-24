// Missing delete -> LeakSanitizer report
#include <iostream>
struct Event {
    int rows; char** map;
    Event(int r, int s) : rows(r), map(new char*[r]) { for (int i = 0; i < r; ++i) map[i] = new char[s]; }
    ~Event() { for (int i = 0; i < rows; ++i) delete[] map[i]; delete[] map; }
};
int main() { Event* e = new Event(10, 20); (void)e; std::cout << "forgot delete\n"; }
