#include "../src/foundobjectprocessor.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"


#include "../src/xmregcore/tests/JsonTx.h"
#include "../src/xmregcore/tests/mocks.h"

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

      if (blockchain_path.empty())
      {
        blockchain_path = get_default_lmdb_folder(nt);
      }


      if (!boost::filesystem::exists(blockchain_path)) 
      {
         FAIL() << blockchain_path << " does not exist!";
      }  

      mcore = make_unique<MicroCore>(blockchain_path, nt);

      jtx = construct_jsontx("d7dcb2daa64b5718dad71778112d48ad62f4d5f54337037c420cb76efdd8a21c");
  }

  unique_ptr<MicroCore> mcore;
  network_type nt {network_type::STAGENET};
  boost::optional<JsonTx> jtx;

public:
  static string blockchain_path;
};

string FOUND_OBJECT_TEST::blockchain_path {};

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

TEST(SUBADDRESS, HasSubaddressIndex)
{
    auto jtx = construct_jsontx("f81ecd0381c0b89f23cffe86a799e924af7b5843c663e8c07db98a14e913585e");

    ASSERT_TRUE(jtx);

    map<string, subaddress_index> expected_idx {
        {"74kQqzoe4wm2zF67wkU9A6HALoHtfbdDYa9Dyw2Cv7w6XipiDmYT4q5GrmzKWWbh5yhGWfZCd35VUgKQa8L2pDacNqu6f1V", 
            {49, 190}},
        {"7AZiDv1bWNkaDvpXAGPRLpGPSCoy6NPvHLsZkW9Bk8a1Vfig5C83ASyL9uAvhy9rDjGeFdGscTEaEdMFeZUqYnUJKDDuiX2",
            {49,53}},    
        {"78bcyasmHaHK6PGvomfoGFJoy1uKv2G7cR7qqmMBYvyddLiumJkTZmxNZCkhgQftNTUMCi4XBbk2n8Ag1QjNtw1fMqPdWdJ",
            {19,70}}, 
        {"7Bg63RCU3LzRqiKXJqrpJjhf4WBdFx6q71mHkx2h5dandfvFZB2AQLj6VyBqJyRNTdLQNHYC9QKtQga9vfpFHqPuL1YeK7D",
            subaddress_index {24,33}},
        {"56heRv2ANffW1Py2kBkJDy8xnWqZsSrgjLygwjua2xc8Wbksead1NK1ehaYpjQhymGK4S8NPL9eLuJ16CuEJDag8Hq3RbPV",
            subaddress_index {0,0}},
    };

    for (auto const& recipient: jtx->recipients)        
    {
        EXPECT_TRUE(bool {recipient.subaddr_idx});
        
        EXPECT_EQ(*recipient.subaddr_idx,
                 expected_idx[recipient.address_str()]);
    }

}



int main(int argc, char **argv)
{
    
    ::testing::InitGoogleTest(&argc, argv);

    //from https://github.com/google/googletest/issues/765#issuecomment-215663692
    //for (int i = 1; i < argc; ++i) 
    //{
         //printf("arg %2d = %s\n", i, argv[i]);
    //}
    
    if (argc == 3 && string {argv[1]}== "-b") 
    {
        cout << "Using blockchain path: " << argv[2] << endl;
      
        if (!boost::filesystem::exists(argv[2])) 
        {
            cerr << argv[2] << " does not exist!" << endl;
            return EXIT_FAILURE;
        }  
        
        FOUND_OBJECT_TEST::blockchain_path = argv[2]; 

    }

    return RUN_ALL_TESTS();
}


