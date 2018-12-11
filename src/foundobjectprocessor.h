#ifndef FOUNDOBJECTPROCESSOR_H
#define FOUNDOBJECTPROCESSOR_H


#include "ext/json.hpp"
#include "src/UniversalIdentifier.hpp"
#include "utils.hpp"

namespace xmreg
{

using json = nlohmann::json;

class FoundObjectProcessor: public boost::static_visitor<json>
{
public:

    using identifier_t = ModularIdentifier<Output, RealInput,
                         LegacyPaymentID, IntegratedPaymentID>;

    FoundObjectProcessor(
            unique_ptr<MicroCore> _mcore,
            unique_ptr<Account> _sender,
            vector<unique_ptr<Account>> _recipients);


    FoundObjectProcessor(
            unique_ptr<MicroCore> _mcore,
            unique_ptr<Account> _sender);

    json operator()(boost::blank) const;

    json operator()(transaction const& tx) const;

    json operator()(block const& blk) const;

protected:
    unique_ptr<MicroCore> mcore;
    unique_ptr<Account> sender;
    vector<unique_ptr<Account>> recipients;

    identifier_t
    construct_identifier(
            transaction const& tx,
            Account const& acc,
            MicroCore* mc) const;
};

}

#endif // FOUNDOBJECTPROCESSOR_H
