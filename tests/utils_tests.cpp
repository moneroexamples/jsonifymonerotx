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
    auto account = make_account(network_type::STAGENET, ""s);
    EXPECT_FALSE(account);
}

TEST(MAKE_ACCOUNT, AddressViewkeySpendky)
{
    string const input {"56heRv2ANffW1Py2kBkJDy8xnWqZsSrgjLygwjua2xc8Wbksead1NK1ehaYpjQhymGK4S8NPL9eLuJ16CuEJDag8Hq3RbPV,b45e6f38b2cd1c667459527decb438cdeadf9c64d93c8bccf40a9bf98943dc09,df0f5720ae0b69454ca7db35db677272c7c19513cd0dc4147b0e00792a10f406"};

    auto account = make_account(network_type::STAGENET, input);

    EXPECT_TRUE(account);
}

}
