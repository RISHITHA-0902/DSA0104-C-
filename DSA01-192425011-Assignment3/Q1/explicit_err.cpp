// Experiment: implicit conversion blocked by explicit
#include <string>
struct Book { explicit Book(const std::string&) {} };
void shelve(const Book&) {}
int main() { shelve(std::string("Gift")); }
