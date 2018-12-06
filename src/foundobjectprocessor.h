#ifndef FOUNDOBJECTPROCESSOR_H
#define FOUNDOBJECTPROCESSOR_H

#include "utils.hpp"

namespace xmreg
{

class FoundObjectProcessor: public boost::static_visitor<>
{
public:
    FoundObjectProcessor();

    void operator()(boost::blank) const;

    void operator()(transaction const& tx) const;

    void operator()(block const& blk) const;
};

}

#endif // FOUNDOBJECTPROCESSOR_H
