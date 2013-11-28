/*
 * (C) Copyright 1996-2013 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#include "eckit/maths/Map.h"
#include "eckit/maths/List.h"

namespace eckit {
namespace maths {

//--------------------------------------------------------------------------------------------

Map::Map( ExpPtr f,  ExpPtr list ) : Function()
{
    push_back(f);
    push_back(list);
}

string Map::returnSignature() const
{
    return List::sig();
}

ExpPtr Map::evaluate( Scope &ctx ) const
{
    ExpPtr f = args(0, ctx, false);

    const List::value_t& list = List::extract( ctx, args(1, ctx, true) );

    const size_t nlist = list.size();

    List::value_t res;
    res.reserve(nlist);

    for( size_t i = 0; i < nlist; ++i )
    {
        ExpPtr e = list[i]->resolve(ctx)->eval(ctx);
        ExpPtr v = f->eval(e);
        res.push_back( v );
    }

    return ExpPtr(new List( res, List::Swap()));
}


void Map::asCode(std::ostream&o) const
{
    o << "maths::map("; printArgs(o); o << ")";
}

ExpPtr Map::cloneWith(args_t &a) const {
    NOTIMP; // Should not be called
}

// Support for count()
bool Map::countable() const
{
    return args(1)->countable();
}

size_t Map::count() const
{
    return args(1)->count();
}

//--------------------------------------------------------------------------------------------

ExpPtr map( ExpPtr f,  ExpPtr list )
{
    return ExpPtr( new Map(f,list) );
}

//--------------------------------------------------------------------------------------------

} // namespace maths
} // namespace eckit
