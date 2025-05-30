#include <cstdint>


class IPv4PrefixSet
{

    public:

    IPv4PrefixSet();
    ~IPv4PrefixSet();

    bool add(const uint32_t &base, const uint8_t maskLenght);
    bool del(const uint32_t &base, const uint8_t maskLenght);
    int check(const uint32_t &ip);

    private:

    struct IPNode
    {
        bool bit = 0;
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
    bool isPrefixValid(const uint32_t &base, const uint8_t maskLenght, const char* ipAsString);
    bool hasSibling(const IPNode* node, bool nodeBit);
    bool detachNodes(IPNode* node);
    bool isDetachable(const IPNode* node);
    void parseToCIDR(const uint32_t &base, const uint8_t maskLenght, char* outBuffer);
    void freeTree(IPNode* node);
};