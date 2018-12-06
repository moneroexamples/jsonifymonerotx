#include "src/utils.hpp"

#include <iostream>

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

auto nettype         = any_cast<network_type>(options["nettype"]);
auto blockchain_path = any_cast<fs::path>(options["blockchain_path"]);
auto hash_str        = any_cast<string>(options["hash"]);

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
xmreg::MicroCore mcore {blockchain_path.string(), nettype};

auto found_object = get_tx_or_blk(mcore, a_hash);

if (!found_object.which())
{
    cerr << "Tx or block with the followihg hash not found: "
         << hash_str;

    return EXIT_SUCCESS;
}

cout << "Program finished.\n";

return EXIT_SUCCESS;
}
