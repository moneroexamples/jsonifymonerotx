#include "src/MicroCore.h"
#include "src/tools.h"

#include "boost/program_options.hpp"
#include <boost/filesystem.hpp>

#include <iostream>
#include <memory>

namespace po = boost::program_options;
namespace fs = boost::filesystem;

using namespace std;
using namespace cryptonote;

int
main(int argc, const char *argv[])
{

    network_type nettype = network_type::STAGENET;

    string blockchain_path = xmreg::get_default_lmdb_folder(nettype);

    string hash_str;
    string sender_str;

    fs::path blckchain_path;

    try
    {
        po::options_description desc("jsonifymonerotx: construct complete json representaiton of monero tx or blk");

        desc.add_options()
            ("help,h", "Help")
            ("nettype,n", po::value<size_t>()->default_value(static_cast<size_t>(nettype)),
             "network type: 0-MAINNET, 1-TESTNET, 2-STAGENET")
            ("blockchain-path,b", po::value<string>()->default_value(blockchain_path),
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
            return EXIT_SUCCESS;
        }

        if (vm.count("blockchain-path"))
            blckchain_path = fs::path {vm["blockchain-path"].as<string>()};

        if (vm.count("hash"))
            hash_str = vm["hash"].as<string>();

        if (vm.count("sender"))
            sender_str = vm["sender"].as<string>();

//        if (vm.count("anglestep"))
//            angle_step = std::min(std::max(vm["anglestep"].as<size_t>(), static_cast<size_t>(1)), static_cast<size_t>(45));

//        if (vm.count("savegray"))
//            grayscale_dir = fs::path {vm["savegray"].as<string>()};

//        if (vm.count("csvfilename"))
//            csvfname = vm["csvfilename"].as<string>();

//        if (vm.count("roseplotsdir"))
//            roseplotsdir = vm["roseplotsdir"].as<string>();

//        if (save_roseplots == false)
//            roseplotsdir = "";
    }
    catch (const po::error &ex)
    {
        std::cerr << ex.what() << '\n';
        return EXIT_FAILURE;
    }


// setup monero logger
mlog_configure(mlog_get_default_log_path(""), true);
mlog_set_log("1");


cout << "Blockchain path: " << blockchain_path << '\n';
cout << "Initializaing MicroCore\n";

xmreg::MicroCore mcore {blockchain_path, nettype};



cout << "Program finished.\n";

return EXIT_SUCCESS;
}
