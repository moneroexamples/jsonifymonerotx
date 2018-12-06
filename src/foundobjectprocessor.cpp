#include "foundobjectprocessor.h"

namespace xmreg
{

FoundObjectProcessor::FoundObjectProcessor()
{

}

void
FoundObjectProcessor::operator()(boost::blank) const
{
    cout << "Do nothing\n";
}

void
FoundObjectProcessor::operator()(transaction const& tx) const
{
    cout << "Save tx into json\n";
}

void
FoundObjectProcessor::operator()(block const& blk) const
{
    cout << "Save block into json\n";
}

}
