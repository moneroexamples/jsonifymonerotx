#include "foundobjectprocessor.h"

namespace xmreg
{

inline string
tx_to_hex(transaction const& tx)
{
    return epee::string_tools
        ::buff_to_hex_nodelimer(t_serializable_object_to_blob(tx));
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
    
    add_basic_data(jtx);

    crypto::hash tx_hash = get_transaction_hash(tx);
    
    jtx["type"] = "transaction"s;
    jtx["tx_hash"] = pod_to_hex(tx_hash);
    jtx["tx_hex"]  = tx_hex;
    jtx["is_ringct"] = (tx.version > 1);
    jtx["rct_type"] = tx.rct_signatures.type;

    jtx["is_coinbase"] = is_coinbase(tx);

    auto blk_data = get_block_data(tx_hash);

    if (!blk_data)
        return jtx;

    add_block_data(*blk_data, jtx);

    add_outputs_data(tx, jtx);

    add_inputs_data(tx, jtx);

    // add decoded outputs and inputs for the sender
    
    if (sender)
    {
        jtx["sender"] = json{};
        
        auto& jsender = jtx["sender"];

        add_basic_acc_data(*sender, jsender);

        auto identifier = construct_identifier(
                tx, *sender, mcore.get());
        identifier.identify();

        add_payment_ids(identifier, jtx);

        decode_outputs(identifier, jsender);

        decode_inputs(identifier, jsender);
    }

    // now add decoded outputs for the recipients

    if (!recipients.empty())
    {
        jtx["recipient"] = json::array();

        auto& jrecipients = jtx["recipient"];

        for (auto const& recpient: recipients)
        {
            jrecipients.push_back(json{});

            auto& jrecipient = jrecipients.back();
            
            add_basic_acc_data(*recpient, jrecipient);

            auto identifier = construct_identifier(
                    tx, *recpient, mcore.get());
            identifier.identify();

            decode_outputs(identifier, jrecipient);
        }
    }

    return jtx;
}

json
FoundObjectProcessor::operator()(block const& blk) const
{
    json jblk;

    block_data blk_data {};

    blk_data.blk = blk;

    crypto::hash blk_hash = get_block_hash(blk);
    
    jblk["type"] = "block"s;
    jblk["blk_hash"]  = pod_to_hex(blk_hash);

    blk_data.height = mcore->get_core()
        .get_db().get_block_height(blk_hash);

    auto blk_comp_data = get_block_complete_data(blk);

    if (!blk_comp_data)
      return jblk;

    blk_data.comp_blk_data = blk_comp_data->first;
    blk_data.comp_block_data_str = blk_comp_data->second;
    
    add_block_data(blk_data, jblk);

    jblk["transactions"] = json::array();

    auto& txs = jblk["transactions"];

    txs.push_back(this->operator()(blk.miner_tx));

    for (auto const& tx_hash: blk.tx_hashes)
    {
        transaction tx;

        if (!mcore->get_tx(tx_hash, tx))
        {
            cerr << "Error getting tx of hash: " 
                 << pod_to_hex(tx_hash) << '\n';
            return jblk;
        }

        txs.push_back(this->operator()(tx));
    }

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
    {
        //cout << "EmptyRealInput\n";
    };

};

FoundObjectProcessor::identifier_t
FoundObjectProcessor::construct_identifier(
        transaction const& tx,
        Account const& acc,
        MicroCore* mc) const
{

    auto outputs = make_unique<Output>(&acc.ai(), &*acc.vk());

    auto integrated_payid 
        = make_unique<IntegratedPaymentID>(&acc.ai(), &*acc.vk());

    auto legacy_payid = make_unique<LegacyPaymentID>(nullptr, nullptr);

    auto inputs = acc.sk() ?
                make_unique<RealInput>(&acc.ai(), &*acc.vk(), &*acc.sk(), mc)
              : unique_ptr<RealInput> {make_unique<EmptyRealInput>()};

    //auto inputs = acc.sk() ?
                //unique_ptr<Input> {make_unique<RealInput>(
                        //&acc.ai(), &*acc.vk(), &*acc.sk(), mc)}
              //: make_unique<Input>(&acc.ai(), &*acc.vk());

    return make_identifier(tx,
                           std::move(outputs),
                           std::move(inputs),
                           std::move(legacy_payid),
                           std::move(integrated_payid));
}


unique_ptr<FoundObjectProcessor::block_data>
FoundObjectProcessor::get_block_data(
    crypto::hash const& tx_hash) const
{

  auto blk_data = make_unique<block_data>();

  try
  {
      blk_data->height = mcore->get_core().get_db()
          .get_tx_block_height(tx_hash);

      blk_data->blk = mcore->get_core().get_db().
          get_block_from_height(blk_data->height);

  }
  catch (std::exception const& e)
  {
      cerr << "Cant get block height: " << tx_hash
           << e.what() << '\n';

      return nullptr;
  }

  auto blk_comp_data = get_block_complete_data(blk_data->blk);

  if (!blk_comp_data)
      return nullptr;

  blk_data->comp_blk_data = blk_comp_data->first;
  blk_data->comp_block_data_str = blk_comp_data->second;

  return blk_data;
}

unique_ptr<pair<block_complete_entry, string>>
FoundObjectProcessor::get_block_complete_data(
        block const& blk) const
{

  auto blk_comp_data 
      = make_unique<pair<block_complete_entry, string>>();

  if (!mcore->get_block_complete_entry(blk, 
              blk_comp_data->first))
  {
      cerr << "Failed to obtain complete block data \n";
      return nullptr;
  }

  if(!epee::serialization::store_t_to_binary(
              blk_comp_data->first, 
              blk_comp_data->second))
  {
      cerr << "Failed to serialize complete_block_data\n";
      return nullptr;
  }

  return blk_comp_data;
}

void
FoundObjectProcessor::add_basic_data(json& jtx) const
{
    jtx["nettype"] = static_cast<size_t>(mcore->get_nettype());
    jtx["_comment"] = "Just a placeholder for some comment if needed later";
}

void
FoundObjectProcessor::add_block_data(
        block_data const& blk_data,
        json& jtx) const
{
    jtx["block"] = epee::string_tools
             ::buff_to_hex_nodelimer(blk_data.comp_block_data_str);

    jtx["block_version"] = json {blk_data.blk.major_version,
                                 blk_data.blk.minor_version};

    jtx["height"] = blk_data.height;
}

void
FoundObjectProcessor::add_payment_ids(
        identifier_t const& identifier,
        json& jtx) const
{
    jtx["payment_id"] = string(64, '0');
    jtx["payment_id8"] = string(16, '0');
    jtx["payment_id8e"] = string(16, '0');

    crypto::hash legacy_payment_id
            = identifier.get<LegacyPaymentID>()->get();

    if (legacy_payment_id != crypto::null_hash)
    {
        jtx["payment_id"] = pod_to_hex(legacy_payment_id);
    }
    else
    {
        // if the tx don't have legacy payment id, it might have
        // integrated id, so we check for it here

        jtx["payment_id8"] = pod_to_hex(identifier
                                        .get<IntegratedPaymentID>()
                                        ->raw());

        crypto::hash8 integrated_payment_id
                = identifier.get<IntegratedPaymentID>()->get();

        if (integrated_payment_id != crypto::null_hash8)
            jtx["payment_id8e"] = pod_to_hex(integrated_payment_id);
    }
}

void
FoundObjectProcessor::add_outputs_data(
        transaction const& tx, json& jtx) const
{

    jtx["outputs"] = json::array();

    for (auto i = 0u; i < tx.vout.size(); ++i)
    {
        // i will act as output indxes in the tx

        if (tx.vout[i].target.type() != typeid(txout_to_key))
            continue;

        // get tx input key
        txout_to_key const& txout_key
                = boost::get<txout_to_key>(tx.vout[i].target);

        uint64_t amount = tx.vout[i].amount;

        jtx["outputs"].push_back(
                    json {
                         {"index", i},
                         {"public_key", pod_to_hex(txout_key.key)},
                         {"amount", amount}
                     });
    }
}

void
FoundObjectProcessor::add_inputs_data(
        transaction const& tx, json& jtx) const
{
    jtx["inputs"] = json::array();

    auto input_no = tx.vin.size();

    for (auto i = 0u; i < input_no; ++i)
    {
        if(tx.vin[i].type() != typeid(txin_to_key))
            continue;

        // get tx input key
        txin_to_key const& in_key
                = boost::get<txin_to_key>(tx.vin[i]);

        // get absolute offsets of mixins
        std::vector<uint64_t> absolute_offsets
                = cryptonote::relative_output_offsets_to_absolute(
                        in_key.key_offsets);

        //tx_out_index is pair::<transaction hash, output index>
        vector<tx_out_index> indices;
        std::vector<output_data_t> mixin_outputs;

        // get tx hashes and indices in the txs for the
        // given outputs of mixins
        //  this cant THROW DB_EXCEPTION
        try
        {
            // get tx of the real output
            mcore->get_core().get_db().get_output_tx_and_index(
                        in_key.amount, absolute_offsets, indices);

            // get mining ouput info
            mcore->get_output_key(
                        in_key.amount,
                        absolute_offsets,
                        mixin_outputs);
        }
        catch (exception const& e)
        {
            cerr << "Cant get ring member tx_out_index for tx \n";
            return;
        }


        jtx["inputs"].push_back(json {{"key_image", pod_to_hex(in_key.k_image)},
                                      {"amount", in_key.amount},
                                      {"absolute_offsets", absolute_offsets},
                                      {"ring_members", json::array()}});

        json& ring_members = jtx["inputs"].back()["ring_members"];


        if (indices.size() != mixin_outputs.size())
        {
            cerr << "indices.size() != mixin_outputs.size()\n";
            return;
        }

        // serialize each mixin tx
        //for (auto const& txi : indices)
        for (size_t i = 0; i < indices.size(); ++i)
{

           tx_out_index const& txi = indices[i];
           output_data_t const& mo = mixin_outputs[i];

           auto const& mixin_tx_hash = txi.first;
           auto const& output_index_in_tx = txi.second;

           transaction mixin_tx;

           if (!mcore->get_tx(mixin_tx_hash, mixin_tx))
           {
               throw std::runtime_error("Cant get tx: "
                       + pod_to_hex(mixin_tx_hash));
           }

           // serialize tx
           string tx_hex = epee::string_tools::buff_to_hex_nodelimer(
                                   t_serializable_object_to_blob(mixin_tx));

           ring_members.push_back(
                   json {
                          {"ouput_pk", pod_to_hex(mo.pubkey)},
                          {"tx_hash", pod_to_hex(mixin_tx_hash)},
                          {"unlock_time", mo.unlock_time},
                          {"height", mo.height},
                          {"commitment", pod_to_hex(mo.commitment)},
                          {"output_index_in_tx", txi.second},
                          {"tx_hex", tx_hex},
                   });

        }

    } // for (auto i = 0u; i < input_no; ++i)
}


void
FoundObjectProcessor::add_basic_acc_data(
        Account& acc, json& jtx) const
{

    jtx = json {
        {"address", acc.ai2str()},
        {"viewkey", acc.vk2str()},
        {"spendkey", acc.sk2str()},
        {"is_subaddress", acc.is_subaddress()},
        {"outputs", json::array()}
    };
}


uint64_t
FoundObjectProcessor::decode_outputs(
        identifier_t const& identifier,
        json& jtx) const
{

    uint64_t total {0};

    auto& joutputs = jtx["outputs"];
    
    for (auto const& output: identifier.get<Output>()->get())
    {
        joutputs.push_back(json {
                output.idx_in_tx, 
                pod_to_hex(output.pub_key), 
                output.amount}); 

        total += output.amount;
    } 

    jtx["total_recieved"] = total;

    return total;
}

uint64_t 
FoundObjectProcessor::decode_inputs(identifier_t const& identifier,
              json& jtx) const
{
    uint64_t total {0};

    jtx["inputs"] = json::array();

    auto& jinputs = jtx["inputs"];

    auto inputs = identifier.get<RealInput>()->get();

    for (auto const& input: inputs)
    {
        jinputs.push_back(json {
            pod_to_hex(input.key_img), 
            pod_to_hex(input.out_pub_key), 
            input.amount}); 
        
        total += input.amount;
    }
    
    jtx["total_spent"] = total;

    return total;
}


}
