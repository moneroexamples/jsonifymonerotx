#ifndef UTILS_HPP
#define UTILS_HPP

#include "src/MicroCore.h"
#include "src/tools.h"
#include "src/Account.h"

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


map<string, boost::any>
process_program_options(int argc, const char *argv[]);


boost::variant<boost::blank, transaction, block>
get_tx_or_blk(MicroCore const& mcore, crypto::hash const& a_hash);

boost::optional<Account>
make_account(network_type ntype,
             string  const& account_info,
             string const& split_by = ",");
}

#endif // UTILS_HPP
