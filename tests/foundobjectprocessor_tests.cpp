#include "../src/foundobjectprocessor.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "mocks.h"

namespace
{
using namespace std;
using namespace cryptonote;
using namespace xmreg;


TEST(FOUND_OBJECT_PROCESSOR, FullAddressInfo)
{
    auto jtx = construct_jsontx("d7dcb2daa64b5718dad71778112d48ad62f4d5f54337037c420cb76efdd8a21c");

    ASSERT_TRUE(jtx);

    auto const& jsender = jtx->jtx["sender"];

    auto acc_str = jsender["address"].get<string>() + ","
                  + jsender["viewkey"].get<string>() + ","
                  + jsender["spendkey"].get<string>();

    auto acc = make_account(acc_str);

    ASSERT_TRUE(acc);
}


TEST(FOUND_OBJECT_PROCESSOR, IncorrectAddressInfo)
{
    auto acc_str = "some gibrish instead of correct sender info";

    auto acc = make_account(acc_str);

    ASSERT_FALSE(acc);
}

TEST(FOUND_OBJECT_PROCESSOR, TestSearchWithFullAccInfo)
{
    auto jtx = construct_jsontx("d7dcb2daa64b5718dad71778112d48ad62f4d5f54337037c420cb76efdd8a21c");

    ASSERT_TRUE(jtx);

    auto const& jsender = jtx->jtx["sender"];

    auto acc_str = jsender["address"].get<string>() + ","
                  + jsender["viewkey"].get<string>() + ","
                  + jsender["spendkey"].get<string>();

    auto acc = make_account(acc_str);

    auto mcore_mock = make_unique<MockMicroCore>();

    FoundObjectProcessor obj_processor {
        std::move(mcore_mock), std::move(acc)};

    auto jsonify_tx = obj_processor(jtx->tx);

    cout << jsonify_tx.dump(4) << '\n';
}

TEST(FOUND_OBJECT_PROCESSOR, TestSearchWithWithoutSpendkey)
{
    auto jtx = construct_jsontx("d7dcb2daa64b5718dad71778112d48ad62f4d5f54337037c420cb76efdd8a21c");

    ASSERT_TRUE(jtx);

    auto const& jsender = jtx->jtx["sender"];

    auto acc_str = jsender["address"].get<string>() + ","
                  + jsender["viewkey"].get<string>();

    auto acc = make_account(acc_str);

    auto mcore_mock = make_unique<MockMicroCore>();

    FoundObjectProcessor obj_processor {
        std::move(mcore_mock), std::move(acc)};

    auto jsonify_tx = obj_processor(jtx->tx);

    cout << jsonify_tx.dump(4) << '\n';
}

}
