///////////////////////////////////////////////////////////////////////////
//
// File:        TIDSocket_Exception.C
// Description: This file contains the Exception class methods
// Rel:         01.00
// Created:     May, 2001
// Author:      Pablo Rodriguez Archilla      ( pra@tid.es   )
//
// Revised:
//
// (C) Copyright 2009 Telefonica Investigacion y Desarrollo
//     S.A.Unipersonal (Telefonica I+D)
//
// This file is part of Morfeo CORBA Platform.
//
// Morfeo CORBA Platform is free software: you can redistribute it and/or
// modify it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the License,
// or (at your option) any later version.
//
// Morfeo CORBA Platform is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with Morfeo CORBA Platform. If not, see
//
//   http://www.gnu.org/licenses
//
// Info about members and contributors of the MORFEO project
// is available at
//
//   http://morfeo-project.org
//
///////////////////////////////////////////////////////////////////////////

#include "TIDSocket.h"


#ifdef TIDSOCKET_HAVE_STD_IOSTREAM_CLASSES

#include <sstream>
#define OSTRINGSTREAM std::ostringstream
using std::ends;

#else

#include <strstream.h>
#define OSTRINGSTREAM ostrstream

#endif


#include <string.h>

namespace TIDSocket
{




//
// Constructor
//
Exception::Exception(const char* msg, int errnum)
    throw()
{
    init(msg, errnum);
}




//
// Destructor
//
Exception::~Exception()
    throw()
{
}




//
// getErrno()
//
const int Exception::getErrno() const
    throw()
{
    return _err;
}




//
// getReason()
//
const char* Exception::getReason() const
    throw()
{
    return _reason.c_str();
}




//
// what()
//
const char* Exception::what() const
    throw()
{
    return _what.c_str();
}




//
// Object initialization
//
void Exception::init(const char* msg, int errnum)
    throw()
{
    // Error code
    _err = errnum;

    // Error description
    string errstr = "";
    if (_err)
    {
        OSTRINGSTREAM buffer;
        buffer << strerror(_err) << " (" << TIDThr::enostr(_err) << ")" << ends;
        errstr = buffer.str();
    }

    // Reason
    _reason = msg;
    if (_reason.empty())
    {
        _reason = errstr;
    }

    // What
    _what = msg;
    if (! errstr.empty())
    {
        OSTRINGSTREAM buffer;
        buffer << msg << ": " << errstr << ends;
        _what = buffer.str();
    }
}




} //namespace TIDSocket




//
// operator<<
//
ostream& operator<< (ostream& os, const TIDSocket::Exception& ex)
{
    string what  = ex.what();
    string delim = (what.empty()) ? "" : ": ";

    os << ex.getName() << delim << what;
    return os;
}
