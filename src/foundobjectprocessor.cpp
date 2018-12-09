#include "foundobjectprocessor.h"

namespace xmreg
{

string
tx_to_hex(transaction const& tx)
{
    return epee::string_tools::buff_to_hex_nodelimer(t_serializable_object_to_blob(tx));
}


FoundObjectProcessor::FoundObjectProcessor(
        MicroCore* _mcore) : mcore {_mcore}
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

    return jtx;
}

json
FoundObjectProcessor::operator()(block const& blk) const
{
    cout << "Save block into json\n";

    json jblk;

    return jblk;
}

}
