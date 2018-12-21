#ifndef FOUNDOBJECTPROCESSOR_H
#define FOUNDOBJECTPROCESSOR_H


#include "ext/json.hpp"
#include "src/UniversalIdentifier.hpp"
#include "utils.hpp"

namespace xmreg
{

using json = nlohmann::json;

class FoundObjectProcessor: public boost::static_visitor<json>
{
public:

    using identifier_t = ModularIdentifier<Output, RealInput,
                         LegacyPaymentID, IntegratedPaymentID>;

    FoundObjectProcessor(
            unique_ptr<MicroCore> _mcore,
            unique_ptr<Account> _sender,
            vector<unique_ptr<Account>> _recipients);


    FoundObjectProcessor(
            unique_ptr<MicroCore> _mcore,
            unique_ptr<Account> _sender);

    json operator()(boost::blank) const;

    json operator()(transaction const& tx) const;

    json operator()(block const& blk) const;

protected:

    struct block_data
    {
      block blk;
      block_complete_entry comp_blk_data;
      string comp_block_data_str;
      uint64_t height;
    };


    unique_ptr<MicroCore> mcore;
    unique_ptr<Account> sender;
    vector<unique_ptr<Account>> recipients;

    identifier_t
    construct_identifier(
            transaction const& tx,
            Account const& acc,
            MicroCore* mc) const;

    void
    add_basic_data(json& jtx) const;

    unique_ptr<block_data>
    get_block_data(crypto::hash const& tx_hash) const;
    

    unique_ptr<pair<block_complete_entry, string>>
    get_block_complete_data(block const& blk) const;

    void
    add_block_data(block_data const& blk_data,
                   json& jtx) const;

    void
    add_payment_ids(identifier_t const& identifier,
                    json& jtx) const;

    void
    add_outputs_data(transaction const& tx, json& jtx) const;

    void
    add_inputs_data(transaction const& tx, json& jtx) const;

    void
    add_basic_acc_data(Account& acc, json& jtx) const;

    uint64_t
    decode_outputs(identifier_t const& identifier,
                   json& jtx) const;

    uint64_t
    decode_inputs(identifier_t const& identifier,
                  json& jtx) const;

};

}

#endif // FOUNDOBJECTPROCESSOR_H
