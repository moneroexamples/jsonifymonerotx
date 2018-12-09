#include "../src/utils.hpp"

#include "gmock/gmock.h"
#include "gtest/gtest.h"


namespace
{
using namespace std;
using namespace cryptonote;
using namespace xmreg;

TEST(MAKE_ACCOUNT, EmptyString)
{
    auto account = make_account(""s);
    EXPECT_FALSE(account);
}

string const address {"56heRv2ANffW1Py2kBkJDy8xnWqZsSrgjLygwjua2xc8Wbksead1NK1ehaYpjQhymGK4S8NPL9eLuJ16CuEJDag8Hq3RbPV"};
string const viewkey {"b45e6f38b2cd1c667459527decb438cdeadf9c64d93c8bccf40a9bf98943dc09"};
string const spendkey {"df0f5720ae0b69454ca7db35db677272c7c19513cd0dc4147b0e00792a10f406"};

TEST(MAKE_ACCOUNT, AddressViewkeySpendky)
{
    string const input = address + ',' + viewkey + ',' + spendkey;

    auto account = make_account(input);

    EXPECT_TRUE(account);
    EXPECT_EQ(account.ai2str(), address);
    EXPECT_EQ(account.vk2str(), viewkey);
    EXPECT_EQ(account.sk2str(), spendkey);
}

TEST(MAKE_ACCOUNT, AddressViewkey)
{
    string const input = address + ',' + viewkey;

    auto account = make_account(input);

    EXPECT_TRUE(account);
    EXPECT_EQ(account.ai2str(), address);
    EXPECT_EQ(account.vk2str(), viewkey);
    EXPECT_FALSE(account.sk());
}

TEST(MAKE_ACCOUNT, Address)
{
    string const input = address;

    auto account = make_account(input);

    EXPECT_TRUE(account);
    EXPECT_EQ(account.ai2str(), address);
    EXPECT_FALSE(account.vk());
    EXPECT_FALSE(account.sk());
}

TEST(MAKE_ACCOUNT, WrongAddress)
{
    string input = "toshortaddress";

    auto account = make_account(input);

    EXPECT_FALSE(account);
}


}
