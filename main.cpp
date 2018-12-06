#include "src/om_log.h"
#include "src/MicroCore.h"
#include "src/tools.h"

#include <iostream>
#include <memory>


using namespace std;
using namespace cryptonote;

int
main(int ac, const char* av[])
{

// setup monero logger
mlog_configure(mlog_get_default_log_path(""), true);
mlog_set_log("1");

el::Configurations defaultConf;

defaultConf.setToDefault();

defaultConf.setGlobally(el::ConfigurationType::ToStandardOutput, "true");

el::Loggers::reconfigureLogger("jsonifyxmrtx", defaultConf);

OMINFO << "Program is starting";

network_type nettype = network_type::STAGENET;


string blockchain_path = xmreg::get_default_lmdb_folder(nettype);

OMINFO << "Blockchain path: " << blockchain_path;


// create blockchainsetup instance and set its parameters
// such as blockchain status monitoring thread parameters

// once we have all the parameters for the blockchain and our backend
// we can create and instance of CurrentBlockchainStatus class.
// we are going to do this through a shared pointer. This way we will
// have only once instance of this class, which we can easly inject
// and pass around other class which need to access blockchain data

xmreg::MicroCore mcore;

OMINFO << "Initializaing MicroCore";
mcore.init(blockchain_path, nettype);


return EXIT_SUCCESS;
}
