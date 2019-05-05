#include "../src/foundobjectprocessor.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "mocks.h"

namespace
{
using namespace std;
using namespace cryptonote;
using namespace xmreg;


class FOUND_OBJECT_TEST : public ::testing::Test
{

protected:

  void SetUp() override
  {
      // limit monero log output
      mlog_configure(mlog_get_default_log_path(""), true);
      mlog_set_log("1");

      blockchain_path = get_default_lmdb_folder(nt);
      mcore = make_unique<MicroCore>(blockchain_path, nt);

      jtx = construct_jsontx("d7dcb2daa64b5718dad71778112d48ad62f4d5f54337037c420cb76efdd8a21c");
  }

  unique_ptr<MicroCore> mcore;
  network_type nt {network_type::STAGENET};
  string blockchain_path;
  boost::optional<JsonTx> jtx;
};

TEST(MAKE_ACCOUNT, FullAddressInfo)
{
    auto jtx = construct_jsontx("d7dcb2daa64b5718dad71778112d48ad62f4d5f54337037c420cb76efdd8a21c");

    ASSERT_TRUE(jtx);

    auto const& jsender = jtx->jtx["sender"];

    auto acc_str = jsender["address"].get<string>() + ","
                  + jsender["viewkey"].get<string>() + ","
                  + jsender["spendkey"].get<string>();

    auto acc = parse_account(acc_str);

    ASSERT_TRUE(acc);
}


TEST(MAKE_ACCOUNT, IncorrectAddressInfo)
{
    auto acc_str = "some gibrish instead of correct sender info";

    auto acc = parse_account(acc_str);

    ASSERT_FALSE(acc);
}

TEST_F(FOUND_OBJECT_TEST, TestSearchWithFullAccInfo)
{

    auto const& jsender = jtx->jtx["sender"];

    auto acc_str = jsender["address"].get<string>() + ","
                  + jsender["viewkey"].get<string>() + ","
                  + jsender["spendkey"].get<string>();

    auto acc = parse_account(acc_str);

    FoundObjectProcessor obj_processor {
        std::move(mcore), std::move(acc)};

    auto jsonify_tx = obj_processor(jtx->tx);

    cout << jsonify_tx.dump(4) << '\n';
}

TEST_F(FOUND_OBJECT_TEST, TestSearchWithWithoutSpendkey)
{
    auto const& jsender = jtx->jtx["sender"];

    auto acc_str = jsender["address"].get<string>() + ","
                  + jsender["viewkey"].get<string>();

    auto acc = parse_account(acc_str);

    FoundObjectProcessor obj_processor {
        std::move(mcore), std::move(acc)};

    auto jsonify_tx = obj_processor(jtx->tx);

    cout << jsonify_tx.dump(4) << '\n';
}

TEST_F(FOUND_OBJECT_TEST, TestSearchWithRecipients)
{

    auto const& jsender = jtx->jtx["sender"];

    auto acc_str = jsender["address"].get<string>() + ","
                  + jsender["viewkey"].get<string>() + ","
                  + jsender["spendkey"].get<string>();

    auto acc = parse_account(acc_str);

    vector<unique_ptr<Account>> recipients;

    for (auto const& recipient: jtx->recipients)        
    {

        cout << recipient << '\n';

        auto acc_str = recipient.address_str() + ","
            + pod_to_hex(recipient.viewkey) + ","
            + pod_to_hex(recipient.spendkey);

        auto acc = parse_account(acc_str);
        
        recipients.push_back(std::move(acc));
    }

    FoundObjectProcessor obj_processor {
        std::move(mcore), std::move(acc), 
            std::move(recipients)};

    auto jsonify_tx = obj_processor(jtx->tx);

    cout << jsonify_tx.dump(4) << '\n';
}


}
