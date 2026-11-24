// throw;  vs  throw e;  -- 'throw e' throws a NEW copy of the STATIC type (slicing)
#include <iostream>
#include <stdexcept>
struct ClaimError : std::runtime_error { using std::runtime_error::runtime_error; };
struct CoverExceeded : ClaimError { using ClaimError::ClaimError; };
void run(bool plain) {
    try {
        try { throw CoverExceeded("exceeds cover"); }
        catch (const ClaimError& e) { std::cout << "audit: " << e.what() << '\n'; if (plain) throw; else throw e; }
    }
    catch (const CoverExceeded&) { std::cout << "  -> caught as CoverExceeded (original object)\n"; }
    catch (const ClaimError&)    { std::cout << "  -> caught only as ClaimError (sliced copy)\n"; }
}
int main() { std::cout << "throw; \n"; run(true); std::cout << "throw e;\n"; run(false); }
