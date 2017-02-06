///////////////////////////////////////////////////////////////////////////
//
// File:        TIDSocket_Inet6Address.C
// Description: This file contains the Inet6Address class methods
// Rel:         01.00
// Created:     Feb, 2008
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
#include <sys/utsname.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip6.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>


#ifndef INET6_ADDRSTRLEN
#define INET6_ADDRSTRLEN     46
#endif




typedef struct in6_addr in6_addr_t;

namespace TIDSocket
{




//
// Constructor
//
Inet6Address::Inet6Address()
    throw()
{
  // Any address
  struct in6_addr any = IN6ADDR_ANY_INIT;

#ifndef __linux
  *((in6_addr_t*) &_ip) = *((in6_addr_t*) &any);
#else
  *((in6_addr_t*) &(_ip.s6_addr)) = *((in6_addr_t*) &(any.s6_addr));
#endif
}




//
// Destructor
//
Inet6Address::~Inet6Address()
    throw()
{
}




//
// Assign operator
//
Inet6Address& Inet6Address::operator= (const Inet6Address& addr)
    throw()
{
#ifndef __linux
  *((in6_addr_t*) &_ip) = *((in6_addr_t*) &(addr._ip));
#else
  *((in6_addr_t*) &(_ip.s6_addr)) = *((in6_addr_t*) &(addr._ip.s6_addr));
#endif
  _hostname = addr._hostname;
  // TODO
  //_ip = addr._ip;
  return *this;
}




//
// Equals operator
//
bool Inet6Address::operator== (const Inet6Address& addr)
    throw()
{
  return IN6_ARE_ADDR_EQUAL(&_ip, &(*((in6_addr*)&(addr._ip))) );
}




bool Inet6Address::operator== (const Inet4Address& addr)
    throw()
{
  return false;
}


//
// getAddress()
//
const unsigned char* Inet6Address::getAddress(size_t& out_len) const
    throw()
{
    // Inicializa el valor de la longitud de la direccion
    out_len = sizeof(_ip);
    
    // Devuelve un puntero que apunta al comienzo de la IP como buffer de bytes
#ifndef __linux
    return (const unsigned char*) &_ip;
#else
    return (const unsigned char*) &(_ip.s6_addr);
#endif
}




//
// getHostAddress()
//
char* Inet6Address::getHostAddress()
    throw()
{
    // Obtiene la representacion textual de la direccion IP
    char buffer[INET6_ADDRSTRLEN];
    string addr = inet_ntop(AF_INET6, &_ip, buffer, INET6_ADDRSTRLEN);

    // Devuelve una copia
    return duplicate(addr);
}




//
// isAnyLocalAddress()
//
bool Inet6Address::isAnyLocalAddress() const
    throw()
{
  return IN6_IS_ADDR_SITELOCAL(&_ip);
}




//
// isAnyLocalAddress()
//
bool Inet6Address::isIPv4CompatibleAddress() const
    throw()
{
  return IN6_IS_ADDR_V4COMPAT(&_ip);
}




//
// isLinkLocalAddress()
//
bool Inet6Address::isLinkLocalAddress() const
    throw()
{
    return IN6_IS_ADDR_LINKLOCAL(&_ip);
}




//
// isLoopbackAddress()
//
bool Inet6Address::isLoopbackAddress() const
    throw()
{
  return IN6_IS_ADDR_LOOPBACK(&_ip);
}




//
// isMCGlobal()
//
bool Inet6Address::isMCGlobal() const
    throw()
{
    return IN6_IS_ADDR_MC_GLOBAL(&_ip);
}




//
// isMCLinkLocal()
//
bool Inet6Address::isMCLinkLocal() const
    throw()
{
    return IN6_IS_ADDR_MC_LINKLOCAL(&_ip);
}




//
// isMCNodeLocal()
//
bool Inet6Address::isMCNodeLocal() const
    throw()
{
    return IN6_IS_ADDR_MC_NODELOCAL(&_ip);
}




//
// isMCOrgLocal()
//
bool Inet6Address::isMCOrgLocal() const
    throw()
{
    return IN6_IS_ADDR_MC_ORGLOCAL(&_ip);
}




//
// isMCSiteLocal()
//
bool Inet6Address::isMCSiteLocal() const
    throw()
{
  return IN6_IS_ADDR_MC_SITELOCAL(&_ip);
}




//
// isMulticastAddress()
//
bool Inet6Address::isMulticastAddress() const
    throw()
{
  return IN6_IS_ADDR_MULTICAST(&_ip);
}




//
// isSiteLocalAddress()
//
bool Inet6Address::isSiteLocalAddress() const
    throw()
{
  return IN6_IS_ADDR_MC_SITELOCAL(&_ip);
}



//
// clone()
//
InetAddress* Inet6Address::clone() const
    throw()

{
  Inet6Address* inet = new Inet6Address();
  *inet = (*this);
  return inet;
}

} //namespace TIDSocket




//
// operator<<
//
ostream& operator<< (ostream& os, const TIDSocket::Inet6Address& inet)
{
    size_t len;
    const unsigned char* addr = inet.getAddress(len);

    char buffer[INET6_ADDRSTRLEN];
    os << inet_ntop(AF_INET6, addr, buffer, INET6_ADDRSTRLEN);
    return os;
}
