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
auto recipients_vstr = any_cast<vector<string>>(options["recipients"]);
auto save_json       = any_cast<bool>(options["save"]);
auto no_display_json = any_cast<bool>(options["display"]);

if (hash_str.empty())
{
    cerr << "Transaction or block hash not given!\n";
    return EXIT_SUCCESS;
}

cout << "Blockchain path: " << blockchain_path << '\n';


// setup monero logger
mlog_configure(mlog_get_default_log_path(""), true);
mlog_set_log("1");

crypto::hash a_hash;

if (!hex_to_pod(hash_str, a_hash))
{
    cerr << "Failed to convert "<< hash_str << " to crypto::hash\n";
    return EXIT_SUCCESS;
}

cout << "Initializaing MicroCore\n";
auto mcore = make_unique<xmreg::MicroCore>(
        blockchain_path.string(), net_type);

auto found_object = get_tx_or_blk(*mcore, a_hash);

if (!found_object.which())
{
    cerr << "Tx or block with the followihg hash not found: "
         << hash_str;

    return EXIT_SUCCESS;
}

auto sender = xmreg::make_account(sender_str);

if (sender)
    cout << "Sender: " << *sender << '\n';
else
    cout << "Sender's info not given or incorrect. "
            "Will proceed without it." << sender_str << '\n';

vector<unique_ptr<xmreg::Account>> recipients;

if (!recipients_vstr.empty())
{
    for (auto const& recipient_str: recipients_vstr)
    {
        auto recpient = xmreg::make_account(recipient_str);

        if (recpient)
            cout << "Recipient: " << *recpient << '\n';
        else
        {
            cout << "Recpient's info not given or incorrect. "
                    "Will proceed without it: " 
                    << recipient_str << '\n';
            continue;
        }

        recipients.push_back(std::move(recpient));
    }
}

xmreg::FoundObjectProcessor obj_processor {
    std::move(mcore), std::move(sender), std::move(recipients)};

auto jobj = boost::apply_visitor(obj_processor, found_object);


if (no_display_json)
{
    cout << jobj.dump(4) << '\n'; 
}

if (save_json)
{
    auto cwd = fs::current_path();

    auto out_fname = cwd 
        / fs::path {jobj["hash"].get<string>() + ".json"};
    
    ofstream of {out_fname.string()};
    
    of << std::setw(4) << jobj << endl;

    cout << "\nJson file written: " << out_fname << '\n';
}

cout << "Program finished.\n";

return EXIT_SUCCESS;
}
