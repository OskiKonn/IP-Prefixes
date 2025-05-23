
#include <cstdint>
#include <iostream>


class IPnode
{
    public:

    IPnode(bool bit);
    IPnode() = default;

    bool bit;
    bool isPrefix = false;
    bool leadsToPrefix = false;
    uint8_t maskLength = 40;
    IPnode* nextLeft = nullptr;
    IPnode* nextRight = nullptr;
    IPnode* parent = nullptr;


};