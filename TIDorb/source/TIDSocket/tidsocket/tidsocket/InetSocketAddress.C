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
