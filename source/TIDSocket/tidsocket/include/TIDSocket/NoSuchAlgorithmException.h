///////////////////////////////////////////////////////////////////////////
//
// File:        TIDSocket_NoSuchAlgorithmException.h
// Description: This file contains the NoSuchAlgorithmException class methods
// Rel:         01.00
// Created:     May, 2009
// Author:      Alvaro Vega Garcia      ( avega@tid.es   )
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


#ifndef __TID_SOCKET_NO_SUCH_ALGORITHM_EXCEPTION_H__
#define __TID_SOCKET_NO_SUCH_ALGORITHM_EXCEPTION_H__


namespace TIDSocket
{


class NoSuchAlgorithmException : public IOException
{
    public:
        NoSuchAlgorithmException(const char* msg = "", int errnum = 0)
            throw() : IOException(msg, errnum) {}

        virtual ~NoSuchAlgorithmException()
            throw() {}

        const char* getName() const
            throw() { return "TIDSocket::NoSuchAlgorithmException"; }
};


} //namespace TIDSocket


#endif //__TID_SOCKET_NO_SUCH_ALGORITHM_EXCEPTION_H__
