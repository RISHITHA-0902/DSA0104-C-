// 'virtual' removed from the pure function: '= 0' is only allowed on virtual functions
#include <string>
class Bill { public: double total() const = 0; };
int main() {}
