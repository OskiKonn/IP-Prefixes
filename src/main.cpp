
#include <iostream>
#include "IPv4PrefixSet.hpp"

using namespace std;

int main()
{

    cout << "---Running sample instructions---\n\n";

    IPv4PrefixSet set = IPv4PrefixSet();
    uint32_t prefix = 0x20408000;

    set.add(0b11000000101010000000000000000000, 24); // 192.168.0.0/24
    set.add(0b11000000101010000000000000010000, 29); // 192.168.0.16/29
    set.add(0x8000000, 1);
    set.add(0xFFFFFFFF, 32);

    std::cout << "\n\nCheck for 192.168.0.0: " << set.check(0b11000000101010000000000000000000); // 192.168.0.0
    std::cout << "\nCheck for 192.168.0.16: " << set.check(0b11000000101010000000000000010000); // 192.168.0.16

    std::cout << "\n";
    set.del(0b11000000101010000000000000000000, 24);
    std::cout << "\n\nCheck for 192.168.0.0: " << set.check(0b11000000101010000000000000000000) << "\n";

    set.add(0b11000000101010000000000000000000, 24);
    std::cout << "\n\nCheck for 192.168.0.0: " << set.check(0b11000000101010000000000000000000);
    std::cout << "\nCheck for 255.255.255.255: " << set.check(0xFFFFFFFF);
    std::cout << "\nCheck for 192.168.0.16: " << set.check(0b11111000101010000000000000010000);
    std::cout << "\n\n";

    
    return 0;
}
