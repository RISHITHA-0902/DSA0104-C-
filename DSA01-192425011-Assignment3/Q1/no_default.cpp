// Experiment: no default constructor -> array of Books does not compile
#include <string>
struct Book { std::string t; int c; Book(const std::string& s, int n) : t(s), c(n) {} };
int main() { Book shelf[3]; (void)shelf; }
