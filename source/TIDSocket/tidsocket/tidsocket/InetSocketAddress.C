///////////////////////////////////////////////////////////////////////////
//
// File:        TIDSocket_InetSocketAddress.C
// Description: This file contains the InetSocketAddress class methods
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




namespace TIDSocket
{




//
// Constructor
//
InetSocketAddress::InetSocketAddress(const InetAddress* inaddr, in_port_t port)
    throw(IllegalArgumentException)
{
    // Inicializa el puerto
    init_port(port);

    // Inicializa la direccion IP
    init_addr(inaddr);
}




//
// Constructor
//
InetSocketAddress::InetSocketAddress(in_port_t port)
    throw(IllegalArgumentException)
{
    // Inicializa el puerto
    init_port(port);

    // Inicializa la direccion IP
    init_addr(NULL);
}




//
// Constructor
//
InetSocketAddress::InetSocketAddress(const char* hostname, in_port_t port)
    throw(IllegalArgumentException)
{
    // Inicializa el puerto
    init_port(port);

    // Intenta resolver la direccion IP de "hostname"
    InetAddress* inaddr = NULL;
    try
    {
        inaddr = InetAddress::getByName(hostname);
    }
    catch(...)
    {
        _resolved = false;
        return;
    }

    // Inicializa la direccion IP y libera el objeto InetAddress
    init_addr(inaddr);
    delete inaddr;
}




//
// Destructor
//
InetSocketAddress::~InetSocketAddress()
    throw()
{
}




//
// Assign operator
//
InetSocketAddress& InetSocketAddress::operator=(const InetSocketAddress& inaddr)
    throw()
{
    _addr     = inaddr._addr;
    _port     = inaddr._port;
    _resolved = inaddr._resolved;
    return *this;
}




//
// Equals operator
//
bool InetSocketAddress::operator== (const InetSocketAddress& inaddr)
    throw()
{
    return ((_port == inaddr._port) &&
            (_resolved == inaddr._resolved) &&
            (_resolved == false || _addr == inaddr._addr));
}




//
// getAddress()
//
const InetAddress& InetSocketAddress::getAddress() const
    throw()
{
    return _addr;
}




//
// getHostName()
//
const char* InetSocketAddress::getHostName()
    throw()
{
    if (isUnresolved())
    {
        return NULL;
    }

    return _addr.getHostName();
}




//
// getPort()
//
in_port_t InetSocketAddress::getPort() const
    throw()
{
    return _port;
}




//
// isUnresolved()
//
bool InetSocketAddress::isUnresolved() const
    throw()
{
    return (_resolved == false);
}




//
// Inicializacion del puerto del objeto
//
void InetSocketAddress::init_port(in_port_t port)
    throw(IllegalArgumentException)
{
    // Comprueba el valor del puerto
    if (port < 0 || port > 65535)
    {
        throw IllegalArgumentException("Port number out of range");
    }

    // Inicializa el valor del puerto
    _port = port;
}




//
// Inicializacion del la direccion IP del objeto
//
void InetSocketAddress::init_addr(const InetAddress* inaddr)
    throw()
{
    // La direccion no esta resuelta inicialmente
    _resolved = false;

    // Si inaddr es nulo, utiliza la direccion comodin
    if (inaddr)
    {
        _addr = *inaddr;
    }
    else
    {
        InetAddress* inet = new InetAddress();
        _addr = *inet;
        delete inet;
    }

    _resolved = true;
}




} //namespace TIDSocket




//
// operator<<
//
ostream& operator<< (ostream& os, const TIDSocket::InetSocketAddress& sock)
{
    os << sock.getAddress() << ":" << sock.getPort();
    return os;
}
