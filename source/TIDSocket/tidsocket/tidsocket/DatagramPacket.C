///////////////////////////////////////////////////////////////////////////
//
// File:        TIDSocket_DatagramPacket.C
// Description: This file contains the DatagramPacket class methods
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




namespace TIDSocket
{




//
// Constructor
//
DatagramPacket::DatagramPacket(unsigned char* buf, size_t buflen, size_t length)
    throw(IllegalArgumentException)
{
    init(buf, buflen, 0, length, NULL);
}





//
// Constructor
//
DatagramPacket::DatagramPacket(unsigned char* buf, size_t buflen, size_t length,
                               const InetAddress& address, in_port_t port)
    throw(IllegalArgumentException)
{
    InetSocketAddress sock(&address, port);
    init(buf, buflen, 0, length, &sock);
}




//
// Constructor
//
DatagramPacket::DatagramPacket(unsigned char* buf, size_t buflen,
                               size_t offset, size_t length)
    throw(IllegalArgumentException)
{
    init(buf, buflen, offset, length, NULL);
}




//
// Constructor
//
DatagramPacket::DatagramPacket(unsigned char* buf, size_t buflen,
                               size_t offset, size_t length,
                               const InetAddress& address, in_port_t port)
    throw(IllegalArgumentException)
{
    InetSocketAddress sock(&address, port);
    init(buf, buflen, offset, length, &sock);
}




//
// Constructor
//
DatagramPacket::DatagramPacket(unsigned char* buf, size_t buflen,
                               size_t offset, size_t length,
                               const SocketAddress& address)
    throw(IllegalArgumentException)
{
    init(buf, buflen, offset, length, (SocketAddress*) &address);
}




//
// Constructor
//
DatagramPacket::DatagramPacket(unsigned char* buf, size_t buflen, size_t length,
                               const SocketAddress& address)
    throw(IllegalArgumentException)
{
    init(buf, buflen, 0, length, (SocketAddress*) &address);
}




//
// Destructor
//
DatagramPacket::~DatagramPacket()
    throw()
{
    delete _address;
}




//
// Object initialization
//
void DatagramPacket::init(unsigned char* buf,
                          size_t         buflen,
                          size_t         offset,
                          size_t         length,
                          SocketAddress* address)
    throw(IllegalArgumentException)
{
    if (offset + length > buflen)
    {
        throw IllegalArgumentException("Illegal offset or length");
    }

    _buffer  = buf;
    _buflen  = buflen;
    _offset  = offset;
    _length  = length;
    _address = NULL;

    if (address)
    {
        InetSocketAddress* addrptr = dynamic_cast<InetSocketAddress*>(address);
        if (addrptr == NULL)
        {
            throw IllegalArgumentException("Invalid SocketAddress");
        }

        const InetAddress& inet = addrptr->getAddress();
        in_port_t          port = addrptr->getPort();

        _address = new InetSocketAddress(&inet, port);
    }
}




//
// getAddress()
//
const InetAddress* DatagramPacket::getAddress() const
    throw()
{
    InetAddress* addr = NULL;

    if (_address)
    {
        addr = (InetAddress*) &(_address->getAddress());
    }

    return addr;
}




//
// getData()
//
const unsigned char* DatagramPacket::getData() const
    throw()
{
    return _buffer;
}




//
// getLength()
//
ssize_t DatagramPacket::getLength() const
    throw()
{
    return _length;
}




//
// getOffset()
//
size_t DatagramPacket::getOffset() const
    throw()
{
    return _offset;
}




//
// getPort()
//
int DatagramPacket::getPort() const
    throw()
{
    int port = -1;

    if (_address)
    {
        port = _address->getPort();
    }

    return port;
}




//
// getSocketAddress()
//
const SocketAddress* DatagramPacket::getSocketAddress() const
    throw()
{
    return (const SocketAddress*) _address;
}




//
// setAddress()
//
void DatagramPacket::setAddress(const InetAddress& iaddr)
    throw()
{
    if (_address)
    {
        int port = getPort();

        delete _address;
        _address = new InetSocketAddress(&iaddr, port);
    }
}




//
// setData()
//
void DatagramPacket::setData(unsigned char* buf, size_t buflen)
    throw(IllegalArgumentException)
{
    setData(buf, buflen, 0, buflen);
}




//
// setData()
//
void DatagramPacket::setData(unsigned char* buf, size_t buflen,
                             size_t offset, size_t length)
    throw(IllegalArgumentException)
{
    if (offset + length > buflen)
    {
        throw IllegalArgumentException("Illegal offset or length");
    }

    _buffer = buf;
    _buflen = buflen;
    _offset = offset;
    _length = length;
}




//
// setLength()
//
void DatagramPacket::setLength(size_t length)
    throw(IllegalArgumentException)
{
    if (_offset + length > _buflen)
    {
        throw IllegalArgumentException("Illegal length");
    }

    _length = length;
}




//
// setPort()
//
void DatagramPacket::setPort(in_port_t iport)
    throw()
{
    if (_address)
    {
        InetSocketAddress* sock = new InetSocketAddress(getAddress(), iport);
        delete _address;
        _address = sock;
    }
}




//
// setSocketAddress()
//
void DatagramPacket::setSocketAddress(const SocketAddress& address)
    throw(IllegalArgumentException)
{
    const InetSocketAddress* addrptr =
        dynamic_cast<const InetSocketAddress*>(&address);

    if (addrptr == NULL)
    {
        throw IllegalArgumentException("Invalid SocketAddress");
    }

    const InetAddress& inet = addrptr->getAddress();
    in_port_t          port = addrptr->getPort();

    delete _address;
    _address = new InetSocketAddress(&inet, port);
}




} //namespace TIDSocket
