/*
 * (C) Copyright 1996-2015 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

/// @author Tiago Quintino
/// @author Simon Smart
/// @date Dec 2015

#include <iosfwd>
#include <fstream>

#include "sandbox/multiplexer/MultiplexerSink.h"
#include "sandbox/multiplexer/DataSink.h"

#include "eckit/exception/Exceptions.h"
#include "eckit/io/Length.h"
#include "eckit/memory/ScopedPtr.h"
#include "eckit/thread/AutoLock.h"
#include "eckit/thread/Mutex.h"

namespace eckit {
namespace multiplexer {

//
// Create the global (singleton) object so it can be found by the DataSinkFactory
DataSinkBuilder<MultiplexerSink> MultiplexerSinkFactorySingleton(std::string("multiplexer"));


// ------------------------------------------------------------------------------------------------


MultiplexerSink::MultiplexerSink()
: is_open_(false) {
    // TODO: Configuration of what gets multiplexed!
    // n.b. If we go to c++11 we should be storing a scoped_ptr
    sinks_.push_back(DataSinkFactory::build("foo"));
}


MultiplexerSink::~MultiplexerSink() {
    if (is_open())
        close();

    // Note that we manually clean up the allocated DataSinks in the list,
    // as they are _not_ stored in scoped pointers (due to the lack of
    // move semantics).
    for (sink_list_t::iterator it = sinks_.begin(); it != sinks_.end(); ++it) {
        delete (*it);
    }

}


void MultiplexerSink::open(const std::string& key) {
    eckit::Log::info() << "[" << *this << "]: open" << std::endl;

    if (is_open())
        throw eckit::SeriousBug("MultiplexerSink: Cannot open multiple times");

    for (sink_list_t::iterator it = sinks_.begin(); it != sinks_.end(); ++it) {
        (*it)->open(key);
    }
    is_open_ = true;
}


void MultiplexerSink::write(const void* buffer, const Length& length) {
    eckit::Log::info() << "[" << *this << "]: write (" << length << ")" << std::endl;

    if (!is_open())
        throw eckit::SeriousBug(std::string("MultiplexerSink: Cannot write without opening"));

    for (sink_list_t::iterator it = sinks_.begin(); it != sinks_.end(); ++it) {
        (*it)->write(buffer, length);
    }
}


void MultiplexerSink::close() {
    eckit::Log::info() << "[" << *this << "]: close" << std::endl;

    for (sink_list_t::iterator it = sinks_.begin(); it != sinks_.end(); ++it) {
        (*it)->close();
    }
    is_open_ = false;
}


bool MultiplexerSink::is_open() const {
    return is_open_;
}


void MultiplexerSink::print(std::ostream& os) const {
    os << "DataSink (MultiplexerSink)";
}



// ------------------------------------------------------------------------------------------------

}  // namespace multiplexer
}  // namespace eckit

