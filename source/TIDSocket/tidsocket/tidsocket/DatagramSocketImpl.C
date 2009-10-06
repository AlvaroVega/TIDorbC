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
void DatagramSocketImpl::connect(const InetAddress& address, in_port_t port)
    throw(SocketException)
{
    // Get a sockaddr struct from pair (address,port)
    struct sockaddr sock;
    socklen_t       size;
    PlainSocketImpl::toSockAddr(address, port, sock, size);

    // Try connection
    if (::connect((int) _fd, &sock, size))
    {
        throw SocketException("connect() error", errno);
    }

    // Get port number to which the socket is bound
    size = (socklen_t) sizeof(sock);
    if (::getsockname((int) _fd, &sock, &size))
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
