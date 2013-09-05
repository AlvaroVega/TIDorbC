///////////////////////////////////////////////////////////////////////////
//
// File:        TIDSocket_SocketImpl.C
// Description: This file contains the SocketImpl class methods
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
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>




namespace TIDSocket
{




//
// Assign operator
//
SocketImpl& SocketImpl::operator= (const SocketImpl& s)
    throw()
{
    //_address   = s._address;
    if (_address)
      delete _address;
    _address   = s._address->clone();
    _fd        = s._fd;
    _localport = s._localport;
    _port      = s._port;
    _timeout   = s._timeout;
    return *this;
}




//
// Equals operator
//
bool SocketImpl::operator== (const SocketImpl& s)
    throw()
{
    //return (_address   == s._address   &&
    return (*_address  == *(s._address)   &&
            _fd        == s._fd        &&
            _localport == s._localport &&
            _port      == s._port      &&
            _timeout   == s._timeout);
}




//
// getFileDescriptor()
//
FileDescriptor SocketImpl::getFileDescriptor() const
    throw()
{
    return _fd;
}




//
// getInetAddress()
//
const InetAddress& SocketImpl::getInetAddress() const
    throw()
{
    //return _address;
    return *_address;
}




//
// getLocalPort()
//
in_port_t SocketImpl::getLocalPort() const
    throw()
{
    return _localport;
}




//
// getPort()
//
in_port_t SocketImpl::getPort() const
    throw()
{
    return _port;
}




//
// shutdownInput()
//
void SocketImpl::shutdownInput()
    throw(IOException)
{
    if (::shutdown((int) _fd, SHUT_RD))
    {
        throw IOException("shutdownInput() error", errno);
    }
}




//
// shutdownOutput()
//
void SocketImpl::shutdownOutput()
    throw(IOException)
{
    if (::shutdown((int) _fd, SHUT_WR))
    {
        throw IOException("shutdownOutput() error", errno);
    }
}




//
// supportsUrgentData()
//
bool SocketImpl::supportsUrgentData()
    throw()
{
    return false;
}




} //namespace TIDSocket
