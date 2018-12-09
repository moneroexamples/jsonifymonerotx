#ifndef FOUNDOBJECTPROCESSOR_H
#define FOUNDOBJECTPROCESSOR_H

#include "utils.hpp"

#include "ext/json.hpp"

namespace xmreg
{

using json = nlohmann::json;

class FoundObjectProcessor: public boost::static_visitor<json>
{
public:
    FoundObjectProcessor(MicroCore* _mcore);

    json operator()(boost::blank) const;

    json operator()(transaction const& tx) const;

    json operator()(block const& blk) const;

protected:
    MicroCore* mcore {nullptr};
};

}

#endif // FOUNDOBJECTPROCESSOR_H
