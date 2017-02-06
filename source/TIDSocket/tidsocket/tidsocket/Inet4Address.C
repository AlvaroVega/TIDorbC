//
// File:        TIDSocket_Inet4Address.C
// Description: This file contains the Inet4Address class methods
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
#include <sys/utsname.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>


#ifndef INET_ADDRSTRLEN
#define INET_ADDRSTRLEN     16
#endif




#define HOSTENT_BUFFER_SIZE 1024U




namespace TIDSocket
{




//
// Constructor
//
Inet4Address::Inet4Address()
    throw()
{
          // Any address
    struct in_addr any = { INADDR_ANY };
#ifndef __linux
    *((in_addr_t*) &_ip) = *((in_addr_t*) &any);
#else
    *((in_addr_t*) &(_ip.s_addr)) = *((in_addr_t*) &(any.s_addr));
#endif
    _hostname = "";
}




//
// Destructor
//
Inet4Address::~Inet4Address()
    throw()
{
}




//
// Assign operator
//
Inet4Address& Inet4Address::operator= (const Inet4Address& addr)
    throw()
{
#ifndef __linux
    *((in_addr_t*) &_ip) = *((in_addr_t*) &(addr._ip));
#else
    *((in_addr_t*) &(_ip.s_addr)) = *((in_addr_t*) &(addr._ip.s_addr));
#endif
    _hostname = addr._hostname;
    return *this;
}




//
// Equals operator
//
bool Inet4Address::operator== (const Inet4Address& addr)
    throw()
{
#ifndef __linux
    return ((*((in_addr_t*) &_ip) == *((in_addr_t*) &(addr._ip)))
#else
    return ((*((in_addr_t*) &(_ip.s_addr)) == *((in_addr_t*) &(addr._ip.s_addr)))
#endif
            && (_hostname == addr._hostname));
}





bool Inet4Address::operator== (const Inet6Address& addr)
    throw()
{
  return false;
}




//
// getAddress()
//
const unsigned char* Inet4Address::getAddress(size_t& out_len) const
    throw()
{
    // Inicializa el valor de la longitud de la direccion
    out_len = sizeof(_ip);

    // Devuelve un puntero que apunta al comienzo de la IP como buffer de bytes
#ifndef __linux
    return (const unsigned char*) &_ip;
#else
    return (const unsigned char*) &(_ip.s_addr);
#endif

    
}




//
// getHostAddress()
//
char* Inet4Address::getHostAddress()
    throw()
{
    // Obtiene la representacion textual de la direccion IP
    char buffer[INET_ADDRSTRLEN];
    string addr = inet_ntop(AF_INET, &_ip, buffer, INET_ADDRSTRLEN);

    // Devuelve una copia
    return duplicate(addr);
}




//
// isAnyLocalAddress()
//
bool Inet4Address::isAnyLocalAddress() const
    throw()
{
    struct in_addr addr = { INADDR_ANY };
#ifndef __linux
    return (*((in_addr_t*) &_ip) == *((in_addr_t*) &addr));
#else
    return (*((in_addr_t*) &(_ip.s_addr)) == *((in_addr_t*) &(addr.s_addr)));
#endif
}




//
// isIPv4CompatibleAddress()
//
bool Inet4Address::isIPv4CompatibleAddress() const
    throw()
{
  return true;
}




//
// isLinkLocalAddress()
//
bool Inet4Address::isLinkLocalAddress() const
    throw()
{
    return false;
}




//
// isLoopbackAddress()
//
bool Inet4Address::isLoopbackAddress() const
    throw()
{
    struct in_addr addr = { INADDR_LOOPBACK };
#ifndef __linux
    return (*((in_addr_t*) &_ip) == *((in_addr_t*) &addr));
#else
    return (*((in_addr_t*) &(_ip.s_addr)) == *((in_addr_t*) &(addr.s_addr)));
#endif
}




//
// isMCGlobal()
//
bool Inet4Address::isMCGlobal() const
    throw()
{
    return false;
}




//
// isMCLinkLocal()
//
bool Inet4Address::isMCLinkLocal() const
    throw()
{
    return false;
}




//
// isMCNodeLocal()
//
bool Inet4Address::isMCNodeLocal() const
    throw()
{
    return false;
}




//
// isMCOrgLocal()
//
bool Inet4Address::isMCOrgLocal() const
    throw()
{
    return false;
}




//
// isMCSiteLocal()
//
bool Inet4Address::isMCSiteLocal() const
    throw()
{
    return false;
}




//
// isMulticastAddress()
//
bool Inet4Address::isMulticastAddress() const
    throw()
{
#ifndef __linux
    in_addr_t* ip_ptr = (in_addr_t*) &_ip;
    return (IN_MULTICAST(*ip_ptr));
#else
    return (IN_MULTICAST(ntohl(_ip.s_addr)));  
#endif

}



//
// isSiteLocalAddress()
//
bool Inet4Address::isSiteLocalAddress() const
    throw()
{
    return false;
}


//
// clone()
//
InetAddress* Inet4Address::clone() const
    throw()

{
  Inet4Address* inet = new Inet4Address();
  *inet = (*this);
  return inet;
}



} //namespace TIDSocket




//
// operator<<
//
ostream& operator<< (ostream& os, const TIDSocket::Inet4Address& inet)
{
    size_t len;
    const unsigned char* addr = inet.getAddress(len);

    char buffer[INET_ADDRSTRLEN];
    os << inet_ntop(AF_INET, addr, buffer, INET_ADDRSTRLEN);
    return os;
}
