
#include "gtest/gtest.h"
#include "IPv4PrefixSet.hpp"


namespace {

class IpPrefixSetTest : public testing::Test
{
    protected:
    
    const uint32_t base1 = 0xC0A86400;  // 192.168.100.0/24
    const uint8_t mask1 = 24;
    const uint32_t base2 = 0xAC800000;  // 172.128.0.0/10
    const uint8_t mask2 = 10;
    const uint32_t base3 = 0x20408000; // 32.64.128.0/20
    const uint8_t mask3 = 20;
    
    IPv4PrefixSet set = IPv4PrefixSet();


    void addSamplePrefixes()
    {
        set.add(base1, mask1);
        set.add(base2, mask2);
        set.add(base3, mask3);
    }
};


TEST_F(IpPrefixSetTest, AddValidPrefix)
{

    EXPECT_TRUE(set.add(base1, mask1));
    EXPECT_TRUE(set.add(base2, mask2));
    EXPECT_TRUE(set.add(base3, mask3));

}


TEST_F(IpPrefixSetTest, AddExistingPrefix)
{

    addSamplePrefixes();

    EXPECT_FALSE(set.add(base1, mask1));
    EXPECT_FALSE(set.add(base2, mask2));
    EXPECT_FALSE(set.add(base3, mask3));

}


TEST_F(IpPrefixSetTest, DeleteExistingPrefix)
{
    
    addSamplePrefixes();

    EXPECT_TRUE(set.del(base1, mask1));
    EXPECT_TRUE(set.del(base2, mask2));
    EXPECT_TRUE(set.del(base3, mask3));

}


TEST_F(IpPrefixSetTest, DeleteNonExistingPrefix)
{

    addSamplePrefixes();

    EXPECT_FALSE(set.del(base1, 26));
    EXPECT_FALSE(set.del(base2, 9));
    EXPECT_FALSE(set.del(base3, 17));
}


TEST_F(IpPrefixSetTest, CheckBelongingIP)
{

    addSamplePrefixes();
    set.add(base1, 27);
    set.add(base1, 30);
    set.add(base2, 24);


    EXPECT_EQ(30, set.check(0xC0A86403));
    EXPECT_EQ(24, set.check(0xAC8000A0));

}


TEST_F(IpPrefixSetTest, CheckNonBelongingIP)
{

    EXPECT_EQ(-1, set.check(0xC0A86403));
    EXPECT_EQ(-1, set.check(0x11111111));
    EXPECT_EQ(-1, set.check(0xFFFFFFFF));

}

}   // namespace

