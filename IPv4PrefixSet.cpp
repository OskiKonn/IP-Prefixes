
#include <iostream>
#include "include/IPv4PrefixSet.hpp"


IPv4PrefixSet::IPv4PrefixSet()
{

    prefixTree = new IPNode();

}

bool IPv4PrefixSet::add(const uint32_t &base, const uint8_t maskLenght)
{

    if (!isPrefixValid(base, maskLenght))
        return false;

    int addedNodes = 0;
    IPNode* currentNode = prefixTree;
    IPNode* nextNode;
    uint8_t depth = 0;

    while (depth < maskLenght)
    {

        bool bit = base & (1u << (31 - depth));


        if (bit)
            nextNode = currentNode->nextRight;   
                  
        else
            nextNode = currentNode->nextLeft;


        if (!nextNode)
        {
            nextNode = new IPNode(bit);
            nextNode->parent = currentNode;

            if (bit)
                currentNode->nextRight = nextNode;
            else
                currentNode->nextLeft = nextNode;

            ++addedNodes;
        }

        currentNode->leadsToPrefix = true;
        currentNode = nextNode;
        ++depth;
    }


    if (currentNode->isPrefix)
        return false;

    currentNode->isPrefix = true;
    currentNode->leadsToPrefix = false;
    currentNode->maskLength = maskLenght;
    
    return true;
}

bool IPv4PrefixSet::del(const uint32_t &base, const uint8_t maskLenght)
{
    IPNode* currentNode = prefixTree;
    uint8_t depth = 0;

    while (depth < maskLenght && currentNode)
    {
        bool bit = base & (1u << (31 - depth));

        if (bit)
            currentNode = currentNode->nextRight;   
                  
        else
            currentNode = currentNode->nextLeft;

        
        ++depth;
    }

    if (!currentNode || !currentNode->isPrefix)
    {
        std::cout << "\n Prefix doesn't exist";
        return false;
    }


    if (currentNode->leadsToPrefix)
    {
        std::cout << "\nPrefix disabled";
        currentNode->isPrefix = false;
        return true;
    }
    else
    {
       return detachNodes(currentNode);
    }

}

int IPv4PrefixSet::check(const uint32_t &ip)
{
    uint8_t depth = 0;
    int maxMaskLength = -1;
    IPNode* currentNode = prefixTree;

    while (depth < 32 && currentNode)
    {
        if (currentNode->isPrefix)
            maxMaskLength = currentNode->maskLength;

        if (ip & (1u << (31 - depth)))
            currentNode = currentNode->nextRight;
        else
            currentNode = currentNode->nextLeft;

        ++depth;
    }


    return maxMaskLength;
}

bool IPv4PrefixSet::isPrefixValid(const uint32_t &base, const uint8_t maskLenght)
{
    if (maskLenght < 0 || maskLenght > 32)
    {
        std::cout << "\nInvalid maskLenght value";
        return false;
    }

    if (base << maskLenght != 0)
    {
        std::cout << "\nInvalid prefix form";
        return false;
    }

    return true;
}


bool IPv4PrefixSet::foundNewPrefix(IPNode* prefix, uint8_t maskLength)
{
    return prefix->isPrefix && prefix->maskLength > maskLength;
}

bool IPv4PrefixSet::detachNodes(IPNode* node)
{
    IPNode* nextNode;
    static int it = 0;
    int nodesDetached = 0;
    it = 0;

    node->isPrefix = false;
    
    while(!node->isPrefix && node && !node->leadsToPrefix)
    {
        if (!node->parent) break; // Don't delete root
        //std::cout << "\nLEADS: " << node->leadsToPrefix << ", " << ++it;

        bool bit = node->bit;

        if (bit)
            node->parent->nextRight = nullptr;
        else
            node->parent->nextLeft = nullptr;

        
        if (!hasSibling(node, bit))
        {
            node->parent->leadsToPrefix = false;
            ++nodesDetached;
        }
            
        nextNode = node->parent;
        delete node;
        node = nextNode;
    }
    
    std::cout << "\nDetached nodes: " << nodesDetached;
    return nodesDetached > 0;

}

bool IPv4PrefixSet::hasSibling(IPNode* node, bool nodeBit)
{
    return (nodeBit) ? (node->parent->nextLeft != nullptr) : (node->parent->nextRight != nullptr);
}

void IPv4PrefixSet::freeTree(IPNode* node)
{
    if (!node) 
        return;

    freeTree(node->nextLeft);
    freeTree(node->nextRight);

    delete node;
}

IPv4PrefixSet::~IPv4PrefixSet()
{
    if (prefixTree)
        freeTree(prefixTree);
}
