/*
 * (C) Copyright 1996-2013 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

/// @file Func.h
/// @author Baudouin Raoult
/// @author Tiago Quintino
/// @date November 2013

#ifndef eckit_maths_Func_h
#define eckit_maths_Func_h

#include "eckit/maths/Exp.h"

namespace eckit {
namespace maths {

//--------------------------------------------------------------------------------------------

/// Represents a Function with some arguments
class Func : public Expression {

public: // types

    typedef std::string key_t;
    typedef boost::function< ValPtr ( const args_t& ) > func_t;

    typedef std::map< key_t, func_t > dispatcher_t;

protected: // methods

    /// Empty contructor is usually used by derived classes that
    /// handle the setup of the parameters themselves
    Func() {}

    /// Contructor taking a list of parameters
    /// handle the setup of the parameters themselves
    Func( const args_t& args ) : Expression(args) {}

public: // methods

    virtual ~Func();

    static dispatcher_t& dispatcher();

    static std::string class_name() { return "Func"; }

public: // virtual methods

    virtual ValPtr evaluate();

    virtual ExpPtr reduce();

    virtual std::string signature() const
    {
        return signature_args( args_ );
    }

    virtual void print( std::ostream& o ) const;

protected: // methods

    std::string signature_args( const args_t& args ) const
    {
        std::ostringstream o;
        o << type_name() << "(";
        for( size_t i = 0; i < args.size(); ++i )
        {
            if(i) o << ",";
            o << args[i]->signature();
        }
        o << ")";
        return o.str();
    }
};

//--------------------------------------------------------------------------------------------

} // namespace maths
} // namespace eckit

#endif
