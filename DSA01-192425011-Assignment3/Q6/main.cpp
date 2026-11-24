// Q6 - Text class for an SMS gateway (manual memory + operator rules)
#include <chrono>
#include <cstring>
#include <iostream>
#include <string>
class Text {
    char* d; size_t len;
public:
    static int copies;
    Text(const char* s = "") : d(new char[std::strlen(s) + 1]), len(std::strlen(s)) { std::memcpy(d, s, len + 1); }
    Text(const Text& o) : d(new char[o.len + 1]), len(o.len) { std::memcpy(d, o.d, len + 1); ++copies; }
    Text& operator=(const Text& o) {
        if (this != &o) {                                    // self-assignment guard
            char* f = new char[o.len + 1]; std::memcpy(f, o.d, o.len + 1);
            delete[] d; d = f; len = o.len;
        }
        return *this;
    }
    ~Text() { delete[] d; }
    Text& operator+=(const Text& o) {
        char* f = new char[len + o.len + 1];
        std::memcpy(f, d, len); std::memcpy(f + len, o.d, o.len + 1);
        delete[] d; d = f; len += o.len; return *this;
    }
    friend Text operator+(const Text& a, const Text& b) { Text r(a); r += b; return r; }
    friend bool operator==(const Text& a, const Text& b) { return a.len == b.len && std::strcmp(a.d, b.d) == 0; }
    friend bool operator!=(const Text& a, const Text& b) { return !(a == b); }
    friend std::ostream& operator<<(std::ostream& os, const Text& t) { return os << t.d; }
    size_t size() const { return len; }
};
int Text::copies = 0;
int main() {
    Text a("Your OTP "), b("is "), c("4821");
    Text::copies = 0;
    Text msg = a + b + c;
    std::cout << msg << "  | copies for a+b+c: " << Text::copies << '\n';
    Text e, e2("");
    std::cout << "empty == empty: " << std::boolalpha << (e == e2) << ", msg != e: " << (msg != e) << '\n';
    msg = msg; std::cout << "after self-assign: " << msg << '\n';
    Text longMsg; for (int i = 0; i < 50; ++i) longMsg += "0123456789";
    std::cout << "long message length: " << longMsg.size() << '\n';
    using C = std::chrono::steady_clock;
    auto t0 = C::now(); Text t; for (int i = 0; i < 100000; ++i) t += "x";
    auto t1 = C::now(); std::string s; for (int i = 0; i < 100000; ++i) s += "x";
    auto t2 = C::now();
    std::cout << "100000 concatenations: Text " << std::chrono::duration<double, std::milli>(t1 - t0).count()
              << " ms, std::string " << std::chrono::duration<double, std::milli>(t2 - t1).count() << " ms\n";
}
