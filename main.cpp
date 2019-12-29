#include "src/foundobjectprocessor.h"

#include <iostream>

using xmreg::operator<<;

using epee::string_tools::hex_to_pod;
using epee::string_tools::pod_to_hex;
using boost::any_cast;

namespace fs = boost::filesystem;

using namespace cryptonote;
using namespace std;


int
main(int argc, const char *argv[])
{

auto options = xmreg::process_program_options(argc, argv);

if (any_cast<bool>(options["error"])
        || any_cast<bool>(options["help"]))
    return EXIT_SUCCESS;

auto net_type        = any_cast<network_type>(options["nettype"]);
auto blockchain_path = any_cast<fs::path>(options["blockchain_path"]);
auto hash_str        = any_cast<string>(options["hash"]);
auto sender_str      = any_cast<string>(options["sender"]);
auto message_str     = any_cast<string>(options["message"]);
auto txprvkey_str    = any_cast<string>(options["txprvkey"]);
auto recipients_vstr = any_cast<vector<string>>(options["recipients"]);
auto subaddress_indices_vstr 
    = any_cast<vector<string>>(options["subaddress_indices"]);
auto save_json       = any_cast<bool>(options["save"]);
auto save_command    = any_cast<bool>(options["command"]);
auto no_display_json = any_cast<bool>(options["display"]);

if (hash_str.empty())
{
    cerr << "Transaction or block hash not given!\n";
    return EXIT_SUCCESS;
}

cout << "Blockchain path: " << blockchain_path << '\n';

if (!boost::filesystem::exists(blockchain_path / "data.mdb"))
{
    cerr << "Blockchain folder: " 
         << blockchain_path 
         << " does not exist!\n";

    return EXIT_FAILURE;
}


// setup monero logger
mlog_configure(mlog_get_default_log_path(""), true);
mlog_set_log("1");

unique_ptr<uint64_t> blk_height;
unique_ptr<crypto::hash> a_hash;

if (hash_str.length() != 64)
{
    // seems we dont have hash. maybe its 
    // a block height. 
    try
    {
        blk_height = make_unique<uint64_t>(
                boost::lexical_cast<uint64_t>(hash_str));
    }
    catch (boost::bad_lexical_cast const& e)
    {
        cerr << "Cant convert " << hash_str
             << " into uint64_t\n";
        return EXIT_SUCCESS;
    }

}
else
{
    // seems we have a hash. so try to decode it
    //
    a_hash = make_unique<crypto::hash>();
     
    if (!hex_to_pod(hash_str, *a_hash))
    {
        cerr << "Failed to convert "<< hash_str << " to crypto::hash\n";
        return EXIT_SUCCESS;
    }
}


cout << "Initializaing MicroCore\n";
auto mcore = make_unique<xmreg::MicroCore>(
        blockchain_path.string(), net_type);

if (blk_height)    
{
    auto current_height =  mcore->get_core().get_current_blockchain_height();

    if (*blk_height > current_height)
    {
        cerr << "Requested block height " << *blk_height 
            <<  "is greater than current height " 
            << current_height << '\n';
        return EXIT_SUCCESS;
    }

    try
    {
        a_hash = make_unique<crypto::hash>(mcore->get_core()
            .get_db().get_block_hash_from_height(*blk_height));
    }
    catch (std::exception const& e)
    {
        cerr << "Failed to get hash of block " << *blk_height << '\n';
        return EXIT_SUCCESS;
    }

    cout << "Found hash for block: " << *blk_height
         << ", " << pod_to_hex(*a_hash) << '\n';
}

auto found_object = get_tx_or_blk(*mcore, *a_hash);

if (!found_object.which())
{
    cerr << "Tx or block with the followihg hash not found: "
         << hash_str;

    return EXIT_SUCCESS;
}

auto sender = xmreg::parse_account(sender_str);

if (sender)
    cout << "Sender: " << *sender << '\n';
else
    cout << "Sender's info not given or incorrect. "
            "Will proceed without it." << sender_str << '\n';

if (sender->type() == xmreg::Account::PRIMARY)
{
    // if we have primary account, we can populate 
    // its subaddresses map
    auto primary_account 
        = static_cast<xmreg::PrimaryAccount*>(sender.get());

    primary_account->populate_subaddress_indices();
}

vector<unique_ptr<xmreg::Account>> recipients;

if (!recipients_vstr.empty())
{
    for (auto const& recipient_str: recipients_vstr)
    {
        auto recpient = xmreg::parse_account(recipient_str);

        if (recpient)
            cout << "Recipient: " << *recpient << '\n';
        else
        {
            cout << "Recpient's info not given or incorrect. "
                    "Will proceed without it: " 
                    << recipient_str << '\n';
            continue;
        }


        //if (recpient->type() == xmreg::Account::PRIMARY)
        //{
            //// if we have primary account, we can populate 
            //// its subaddresses map
            
            //auto primary_account 
                //= static_cast<xmreg::PrimaryAccount*>(recpient.get());

            //primary_account->populate_subaddress_indices();
        //}

        recipients.push_back(std::move(recpient));
    }
}

vector<subaddress_index> subaddress_indices; 

if (!subaddress_indices_vstr.empty())
{
    for (auto const& idx_str: subaddress_indices_vstr)
    {
        auto subaddr_idx = xmreg::parse_subaddress_index(idx_str);

        if (!subaddr_idx)
            return EXIT_SUCCESS;

        subaddress_indices.push_back(*subaddr_idx);
    }
    

    if (subaddress_indices.size() != recipients.size())
    {
        cerr << "Number of recipients is differnent than "
             << "number of subaddress indices provided\n";
        return EXIT_SUCCESS;
    }

    // set indices for each account. Assuming we have 
    // correct order 
    
    for (size_t i {0}; i < recipients.size(); ++i)
    {
        recipients[i]->set_index(subaddress_indices[i]);
    }
}


xmreg::FoundObjectProcessor obj_processor {
    std::move(mcore), std::move(sender), 
    std::move(recipients)};

auto jobj = boost::apply_visitor(obj_processor, found_object);

if (!txprvkey_str.empty())
{
    // before we store tx private key,
    // just check if it is valid
    crypto::secret_key tx_prv_key;

    if (!hex_to_pod(txprvkey_str, tx_prv_key))
    {
        cerr << "Tx private key '"
             << txprvkey_str <<"' has wrong format\n";
        return EXIT_SUCCESS;
    }

    jobj["tx_prv_key"] = txprvkey_str;
}

if (save_command)
{
    jobj["generated_using"] 
        = any_cast<string>(options["generated_using"]);
}

if (!message_str.empty())
{
    jobj["_comment"] = message_str;
}

if (no_display_json)
{
    cout << jobj.dump(4) << '\n'; 
}



if (save_json)
{
    auto cwd = fs::current_path();

    fs::path out_fname {cwd};

    if (jobj["type"] == "transaction"s)
        out_fname /= fs::path {
            "tx_"s + jobj["tx_hash"].get<string>() 
                + ".json"};
    else
        out_fname /= fs::path {
            "blk_"s + 
            std::to_string(jobj["height"].get<uint64_t>()) 
                + ".json"};
    
    ofstream of {out_fname.string()};
    
    of << std::setw(4) << jobj << endl;

    cout << "\nJson file written: " << out_fname << '\n';
}

cout << "Program finished.\n";

return EXIT_SUCCESS;
}
