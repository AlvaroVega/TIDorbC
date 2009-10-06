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
// Copyright 2001 Telefonica, I+D. Printed in Spain (Europe). All Rights
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
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
