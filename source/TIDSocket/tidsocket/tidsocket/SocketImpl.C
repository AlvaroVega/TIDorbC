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
    _address   = s._address;
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
    return (_address   == s._address   &&
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
    return _address;
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
