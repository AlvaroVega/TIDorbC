///////////////////////////////////////////////////////////////////////////
//
// File:        TIDSocket_Exception.h
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


#ifndef __TID_SOCKET_EXCEPTION_H__
#define __TID_SOCKET_EXCEPTION_H__


#include <exception>
#include <string>


#ifdef TIDSOCKET_HAVE_STD_IOSTREAM_CLASSES
#include <iostream>
#else
#include <iostream.h>
#endif


#ifdef TIDSOCKET_HAVE_NAMESPACE_STD
using std::exception;
using std::string;
#ifdef TIDSOCKET_HAVE_STD_IOSTREAM_CLASSES
using std::ostream;
#endif
#endif


namespace TIDSocket
{


class Exception : public exception
{
    public:
        Exception(const char* msg = "", int errnum = 0)
            throw();

        virtual ~Exception()
            throw();

        virtual const char* getName() const
            throw() = 0;

        const int getErrno() const
            throw();

        const char* getReason() const
            throw();

        const char* what() const
            throw();

    protected:
        int    _err;
        string _reason;
        string _what;

    private:
        void init(const char* msg, int errnum)
            throw();
};


} //namespace TIDSocket


ostream& operator<< (ostream& os, const TIDSocket::Exception& ex);


#endif //__TID_SOCKET_EXCEPTION_H__
