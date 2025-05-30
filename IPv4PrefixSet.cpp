
#include <iostream>
#include <chrono>
#include "include/IPv4PrefixSet.hpp"


IPv4PrefixSet::IPv4PrefixSet()
{

    prefixTree = new IPNode();

}



bool IPv4PrefixSet::add(const uint32_t &base, const uint8_t maskLenght)
{
    char ipString[19];
    parseToCIDR(base, maskLenght, ipString);


    if (!isPrefixValid(base, maskLenght, ipString))
        return false;


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

        }


        currentNode->leadsToPrefix = true;
        currentNode = nextNode;
        ++depth;
    }


    if (currentNode->isPrefix)
    {
        std::cout << "\nPrefix " << ipString << " already exists";
        return false; 
    }


    currentNode->isPrefix = true;
    currentNode->leadsToPrefix = false;
    currentNode->maskLength = maskLenght;

    std::cout << "\nPrefix " << ipString <<  " added successfully";
    
    return true;
}



bool IPv4PrefixSet::del(const uint32_t &base, const uint8_t maskLenght)
{

    char ipString[19];
    parseToCIDR(base, maskLenght, ipString);


    if (!isPrefixValid(base, maskLenght, ipString))
        return false;


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
        std::cout << "\n Prefix " << ipString << " doesn't exist";
        return false;
    }


    if (currentNode->leadsToPrefix)
    {
        std::cout << "\nPrefix " << ipString << " disabled";
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
    auto start = std::chrono::high_resolution_clock::now();

    uint8_t depth = 0;
    int maxMaskLength = -1;
    IPNode* currentNode = prefixTree;

    while (depth <= 32 && currentNode)
    {
        if (currentNode->isPrefix)
            maxMaskLength = currentNode->maskLength;

        if (ip & (1u << (31 - depth)))
            currentNode = currentNode->nextRight;
        else
            currentNode = currentNode->nextLeft;

        ++depth;
    }


    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "\nCheck elapsed time: " << duration.count() << " microseconds\n";

    return maxMaskLength;
}



bool IPv4PrefixSet::isPrefixValid(const uint32_t &base, const uint8_t maskLenght, const char* ipAsString)
{
    if (maskLenght > 32)
    {
        std::cout << "\n" << ipAsString << ": " <<  " invalid mask length";
        return false;
    }


    if (maskLenght != 32 && base << maskLenght != 0)
    {
        std::cout << "\n" << ipAsString << ": " << " invalid prefix form";
        return false;
    }

    return true;
}



bool IPv4PrefixSet::detachNodes(IPNode* node)
{
    IPNode* nextNode;
    unsigned int nodesDetached = 0;

    node->isPrefix = false;
    
    while(isDetachable(node))
    {
        if (!node->parent) break; // Don't delete root

        bool bit = node->bit;

        if (bit)
            node->parent->nextRight = nullptr;
        else
            node->parent->nextLeft = nullptr;

        
        if (!hasSibling(node, bit))
        {
            node->parent->leadsToPrefix = false;
        }
            
        nextNode = node->parent;
        delete node;
        ++nodesDetached;
        node = nextNode;
    }
    
    std::cout << "\nNodes removed: " << nodesDetached;
    return nodesDetached > 0;

}



bool IPv4PrefixSet::hasSibling(const IPNode* node, const bool nodeBit)
{
    return (nodeBit) ? (node->parent->nextLeft != nullptr) : (node->parent->nextRight != nullptr);
}



bool IPv4PrefixSet::isDetachable(const IPNode* node)
{
    return node && !node->isPrefix && !node->leadsToPrefix;
}



void IPv4PrefixSet::parseToCIDR(const uint32_t &base, const uint8_t maskLenght, char* outBuffer)
{
    uint8_t octet1 = (base >> 24) & 0xFF;
    uint8_t octet2 = (base >> 16) & 0xFF;
    uint8_t octet3 = (base >> 8) & 0xFF;
    uint8_t octet4 = base & 0xFF;

    sprintf(outBuffer, "%u.%u.%u.%u/%u", octet1, octet2, octet3, octet4, maskLenght);

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
