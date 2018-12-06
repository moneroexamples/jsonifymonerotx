#ifndef UTILS_HPP
#define UTILS_HPP

#include "src/MicroCore.h"
#include "src/tools.h"

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/optional.hpp>
#include <boost/variant.hpp>

#include <map>
#include <string>

namespace xmreg
{

using epee::string_tools::hex_to_pod;
using epee::string_tools::pod_to_hex;
using boost::any_cast;

namespace fs = boost::filesystem;
namespace po = boost::program_options;

using namespace cryptonote;
using namespace crypto;
using namespace std;


auto
process_program_options(int argc, const char *argv[])
{
    map<string, boost::any> options;

    options["help"] = false;
    options["error"] = false;
    options["nettype"] = network_type::STAGENET;

    options["blockchain_path"] = xmreg::get_default_lmdb_folder(
                any_cast<network_type>(options["nettype"]));

    string hash_str;
    string sender_str;

    fs::path blckchain_path;

    try
    {
        po::options_description desc("jsonifymonerotx: construct complete json representaiton of monero tx or blk");

        desc.add_options()
            ("help,h", "Help")
            ("hash", po::value<string>(),
             "transaction or block hash to jsonify")
            ("nettype,n", po::value<size_t>()->default_value(
                 static_cast<size_t>(any_cast<network_type>(options["nettype"]))),
             "network type: 0-MAINNET, 1-TESTNET, 2-STAGENET")
            ("blockchain-path,b", po::value<string>()->default_value(
                 any_cast<string>(options["blockchain_path"])),
             "Path to lmdb folder containing the blockchain")
            ("sender,s", po::value<string>(),
            "Optinal sender's address,viewkey,spendkey");

        po::positional_options_description pos_desc;
        pos_desc.add("hash", -1);

        po::command_line_parser parser{argc, argv};
        parser.options(desc).positional(pos_desc).allow_unregistered();
        po::parsed_options parsed_options = parser.run();

        po::variables_map vm;
        store(parsed_options, vm);
        notify(vm);

        if (vm.count("help"))
        {
            cout << desc << '\n';
            options["help"] = true;
            return options;
        }

        if (vm.count("blockchain-path"))
            options["blockchain_path"]
                    = fs::path {vm["blockchain-path"].as<string>()};

        if (vm.count("hash"))
            options["hash"] = vm["hash"].as<string>();

        if (vm.count("sender"))
            options["sender"] = vm["sender"].as<string>();
    }
    catch (po::error const& ex)
    {
        cerr << ex.what() << '\n';
    }

    return options;
}


auto
get_tx_or_blk(MicroCore const& mcore, crypto::hash const& a_hash)
{
    boost::variant<boost::blank, transaction, block> found_object;

    if (mcore.get_core().have_tx(a_hash))
    {
        found_object = transaction();

        if (!mcore.get_tx(a_hash, boost::get<transaction>(found_object)))
        {
             return found_object;
        }

        cout << "Found transaction with the following hash: "
             << pod_to_hex(a_hash) << '\n';
    }
    else
    {
        found_object = block();

        if (!mcore.get_core()
                .get_block_by_hash(a_hash, boost::get<block>(found_object)))
        {
             return found_object;
        }

        cout << "Found block with the following hash: "
             << pod_to_hex(a_hash) << '\n';
    }

    return found_object;
}

}

#endif // UTILS_HPP
