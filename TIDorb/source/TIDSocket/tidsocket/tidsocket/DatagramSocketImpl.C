///////////////////////////////////////////////////////////////////////////
//
// File:        TIDSocket_DatagramSocketImpl.C
// Description: This file contains the DatagramSocketImpl class methods
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
#include "PlainSocketImpl.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <errno.h>




namespace TIDSocket
{




//
// Destructor
//
DatagramSocketImpl::~DatagramSocketImpl()
    throw()
{
}




//
// Assign operator
//
DatagramSocketImpl& DatagramSocketImpl::operator= (const DatagramSocketImpl& s)
    throw()
{
    _fd        = s._fd;
    _localport = s._localport;
    return *this;
}




//
// Equals operator
//
bool DatagramSocketImpl::operator== (const DatagramSocketImpl& s)
    throw()
{
    return (_fd == s._fd && _localport == s._localport);
}




//
// connect()
//
void DatagramSocketImpl::connect(const InetAddress& address, in_port_t port, const char* interface)
    throw(SocketException)
{
    // Get a sockaddr struct from pair (address,port)
    struct sockaddr_storage sock;
    socklen_t       size;
    PlainSocketImpl::toSockAddr(address, port, sock, size,interface);

    // Try connection
    if (::connect((int) _fd, (struct sockaddr*)&sock, size))
    {
        throw SocketException("connect() error", errno);
    }

    // Get port number to which the socket is bound
    size = (socklen_t) sizeof(sock);
    if (::getsockname((int) _fd, (struct sockaddr*)&sock, &size))
    {
        throw SocketException("connect() error", errno);
    }
    _localport = (in_port_t) ntohs(((struct sockaddr_in*) &sock)->sin_port);
}




//
// disconnect()
//
void DatagramSocketImpl::disconnect()
    throw()
{
    // Get a sockaddr struct for the null address
    struct sockaddr sock;
    socklen_t       size;
    PlainSocketImpl::toNullSockAddr(sock, size);

    // Try disconnection and update local port number
    if (::connect((int) _fd, &sock, size) == 0)
    {
        _localport = PlainSocketImpl::ANY_PORT;
    }
}




//
// getFileDescriptor()
//
FileDescriptor DatagramSocketImpl::getFileDescriptor() const
    throw()
{
    return _fd;
}




//
// getLocalPort()
//
in_port_t DatagramSocketImpl::getLocalPort() const
    throw()
{
    return _localport;
}




} //namespace TIDSocket
