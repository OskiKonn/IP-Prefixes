
#include <iostream>
#include "include/IPv4PrefixSet.hpp"

using namespace std;

int main()
{

    cout << "Hello World!";

    IPv4PrefixSet set = IPv4PrefixSet();
    uint32_t prefix = 0x20408000;
    // cout << "\nDodawanie: " << set.add(prefix, 20);
    // cout << "\nDodawanie: " << set.add(prefix, 18);
    // cout << "\nDodawanie2: " << set.add(prefix, 24);
    // cout << "\nWynik: " << set.check(0x2040801E);
    // cout << "\nWynik2: " << set.check(0x20409123);
    cout << "\n\nTEST 1:\n";

    set.add(0b11000000101010000000000000000000, 24); // 192.168.0.0/24
    set.add(0b11000000101010000000000000000000, 25); // 192.168.0.0/25
    set.add(0b11000000101010000000000010000000, 26); // 192.168.0.128/26

    std::cout << set.check(0b11000000101010000000000000000001) << "\n"; // 192.168.0.1
    std::cout << set.check(0b11000000101010000000000010000001) << "\n"; // 192.168.0.129
    std::cout << set.check(0b11000000101010000000000100000001) << "\n"; // 192.168.1.1

    cout << "\n\nTEST 2:\n";

    set.del(0b11000000101010000000000010000000, 26); // 192.168.0.128/26

    std::cout << "\n" << set.check(0b11000000101010000000000010000001) << "\n"; // 192.168.0.129

    cout << "\n\nTEST3: \n";

    set.del(0b11000000101010000000000000000000, 25); // 192.168.0.0/25

    std::cout << "\n" << set.check(0b11000000101010000000000000000001) << "\n"; // 192.168.0.1

    cout << "\n\nTEST4: \n";

    set.del(0b11000000101010000000000000000000, 24); // 192.168.0.0/24

    std::cout << "\n" << set.check(0b11000000101010000000000000000001) << "\n"; // 192.168.0.1

    cout << "\n\nTEST5: \n";

    set.add(0b10101100000100000000000000000000, 16); // 172.16.0.0/16

    std::cout << "\n" << set.check(0b10101100000100000000111100001111) << "\n"; // 172.16.15.15
    std::cout << "\n" << set.check(0b10101100001000000000111100001111) << "\n"; // 172.32.15.15
    
    return 0;
}
