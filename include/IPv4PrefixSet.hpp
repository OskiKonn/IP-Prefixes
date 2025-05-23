#include <cstdint>
#include <string>


class IPv4PrefixSet
{

    public:

    IPv4PrefixSet();
    ~IPv4PrefixSet();

    bool add(const uint32_t &base, const uint8_t maskLenght);
    bool del(const uint32_t &base, const uint8_t maskLenght);
    bool isPrefixValid(const uint32_t &base, const uint8_t maskLenght);
    int check(const uint32_t &ip);

    private:

    struct IPNode
    {
        bool bit;
        bool isPrefix = false;
        bool leadsToPrefix = false;
        uint8_t maskLength = 40;
        IPNode* nextLeft = nullptr;
        IPNode* nextRight = nullptr;
        IPNode* parent = nullptr;

        IPNode(bool new_bit) : bit(new_bit) {}
        IPNode() {}
    };

    IPNode* prefixTree = nullptr;
    bool foundNewPrefix(IPNode* node, uint8_t maskLength);
    bool hasSibling(IPNode* node, bool nodeBit);
    bool detachNodes(IPNode* node);
    void freeTree(IPNode* node);
};