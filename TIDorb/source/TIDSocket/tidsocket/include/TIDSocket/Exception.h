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
