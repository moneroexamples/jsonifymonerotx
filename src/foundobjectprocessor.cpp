#include "foundobjectprocessor.h"

namespace xmreg
{

string
tx_to_hex(transaction const& tx)
{
    return epee::string_tools::buff_to_hex_nodelimer(t_serializable_object_to_blob(tx));
}


FoundObjectProcessor::FoundObjectProcessor(
        unique_ptr<MicroCore> _mcore,
        unique_ptr<Account> _sender,
        vector<unique_ptr<Account>> _recipients)
    : mcore {std::move(_mcore)},
      sender {std::move(_sender)},
      recipients {std::move(_recipients)}
{

}

FoundObjectProcessor::FoundObjectProcessor(
        unique_ptr<MicroCore> _mcore,
        unique_ptr<Account> _sender)
    : mcore {std::move(_mcore)},
      sender {std::move(_sender)}
{

}

json
FoundObjectProcessor::operator()(boost::blank) const
{
    cout << "Do nothing\n";
    return {};
}

json
FoundObjectProcessor::operator()(transaction const& tx) const
{
    cout << "Save tx into json\n";

    json jtx;

    string tx_hex;

    try
    {
        tx_hex = tx_to_hex(tx);
    }
    catch (std::exception const& e)
    {
        cerr << e.what() << endl;
        return json {"error", "Failed to obtain hex of tx"};
    }

    crypto::hash tx_hash = get_transaction_hash(tx);

    jtx["hash"] = pod_to_hex(tx_hash);
    jtx["hex"]  = tx_hex;
    jtx["nettype"] = static_cast<size_t>(mcore->get_nettype());
    jtx["is_ringct"] = (tx.version > 1);
    jtx["rct_type"] = tx.rct_signatures.type;

    jtx["_comment"] = "Just a placeholder for some comment if needed later";

    auto identifier = construct_identifier(tx, *sender, mcore.get());

    identifier.identify();

    return jtx;
}

json
FoundObjectProcessor::operator()(block const& blk) const
{
    cout << "Save block into json\n";

    json jblk;

    return jblk;
}

// if we dont have spendkey, we are just going using
// this dummy RealInput class in the modular identifier
// which does not do anything
class EmptyRealInput: public RealInput
{
public:

    EmptyRealInput()
        : RealInput(nullptr, nullptr, nullptr, nullptr) {};

    void identify(transaction const& tx,
                  public_key const& tx_pub_key,
                  vector<public_key> const& additional_tx_pub_keys
                  = vector<public_key>{}) override
    {};
};

FoundObjectProcessor::identifier_t
FoundObjectProcessor::construct_identifier(
        transaction const& tx,
        Account const& acc,
        MicroCore* mc) const
{

    auto outputs = make_unique<Output>(&acc.ai(), &*acc.vk());

    auto integrated_payid = make_unique<IntegratedPaymentID>(&acc.ai(), &*acc.vk());

    auto legacy_payid = make_unique<LegacyPaymentID>(nullptr, nullptr);

    auto inputs = acc.sk() ?
                make_unique<RealInput>(&acc.ai(), &*acc.vk(), &*acc.sk(), mc)
              : unique_ptr<RealInput> {make_unique<EmptyRealInput>()};


    return make_identifier(tx,
                           std::move(outputs),
                           std::move(inputs),
                           std::move(legacy_payid),
                           std::move(integrated_payid));
}

}
