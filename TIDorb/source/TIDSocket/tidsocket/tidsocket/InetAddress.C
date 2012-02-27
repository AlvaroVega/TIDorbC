///////////////////////////////////////////////////////////////////////////
//
// File:        InetAddress.C
// Description: This file contains the DatagramSocket class methods
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

#ifndef INET6_ADDRSTRLEN
#define INET6_ADDRSTRLEN     46
#endif


#define HOSTENT_BUFFER_SIZE 1024U




namespace TIDSocket
{




//
// Constructor
//
InetAddress::InetAddress()
    throw()
{
    _hostname = "";
}




//
// Destructor
//
InetAddress::~InetAddress()
    throw()
{
}



//
// Equals operator
//
bool InetAddress::operator== (const InetAddress& addr)
    throw()
{
  // TODO
  return false;
}



//
// duplicate()
//
char* InetAddress::duplicate(const string& str)
    throw()
{
    size_t len = str.length();

    char* result = new char[len + 1];
    result[len] = '\0';

    str.copy(result, len);
    return result;
}



//
// getHostName()
//
const char* InetAddress::getHostName()
    throw()
{
    // Comprueba si el atributo _hostname esta inicializado (longitud > 0);
    // si no lo esta, consulta el nombre asociado a la direccion IP
    if (_hostname.empty())
    {
        char* name = getCanonicalHostName();
        _hostname = (const char*) name;
        delete[] name;
    }

    return _hostname.c_str();
}




//
// getCanonicalHostName()
//
char* InetAddress::getCanonicalHostName()
    throw()
{
    // Obtiene un puntero a la direccion IP
    size_t len;
    const unsigned char* ip_ptr = getAddress(len); // TODO: need to review: may be IPv6

    // TODO: change to getnameinfo()
    struct sockaddr *sa;    /* input */
    sa = (struct sockaddr*) ip_ptr;

    char hbuf[NI_MAXHOST];
    char sbuf[NI_MAXSERV];
    
    int error = getnameinfo(sa, len, 
                            hbuf, NI_MAXHOST, 
                            sbuf, NI_MAXSERV, 
                            NI_NUMERICHOST | NI_NUMERICSERV);

//     if (error != 0)
//       throw UnknownHostException(gai_strerror(error));

    string canonical = (!error) ? (const char*) hbuf : "";

//     // Consulta el nombre del host a partir de la direccion IP; en caso de error
//     // devuelve "" (ya que este metodo no genera ninguna excepcion a tal efecto)
//     struct hostent ht, *ht_ptr;
//     char           ht_buffer[HOSTENT_BUFFER_SIZE];
//     int            ht_error;


// #ifdef __sun
//     ht_ptr = gethostbyaddr_r((const char*) ip_ptr, len, AF_INET,
//                              &ht, ht_buffer, HOSTENT_BUFFER_SIZE, &ht_error);
// #else
//     ht_ptr = gethostbyaddr((const void*) ip_ptr, len, AF_INET);
//     ht_error = h_errno;
// #endif

//     string canonical = (ht_ptr) ? (const char*) ht_ptr->h_name : "";

    // Devuelve una copia
    return duplicate(canonical);
}





//
// static getLocalHost()
//
InetAddress* InetAddress::getLocalHost()
    throw(UnknownHostException)
{
    struct utsname hostdata;

    // Averigua el nombre del host local
    if (uname(&hostdata) == -1)
    {
        throw UnknownHostException("getLocalHost() error", errno);
    }

    return InetAddress::getByName((const char*) hostdata.nodename);
}




//
// static getByName()
//
InetAddress* InetAddress::getByName(const char* host)
    throw(UnknownHostException)
{
	InetAddress* inet = NULL;

    // Consulta todas las direcciones IP del host
    InetAddressList* list = InetAddress::getAllByName(host);
    size_t list_size = list->size();
    
    if (list_size == 0)
    {
        delete list;
        throw UnknownHostException("getByName() error");
    }

    // Copia la primera direcci�n
    //inet = new InetAddress(list->at(0));
    inet = list->at(0);

    // Descarta el resto de la lista
    for (size_t i = 1; i < list_size; i++) {
      InetAddress* aux = list->at(i);
      delete aux;
    }
    delete list;

    return inet;
}




//
// static getAllByName()
//
InetAddressList* InetAddress::getAllByName(const char* host)
    throw(UnknownHostException)
{
	InetAddressList* list = NULL;
    InetAddress*     inet = NULL;

    // Si host es NULL, devuelve la direcci�n de bucle local
    if (host == NULL)
      {
        // IPv4
        struct in_addr  loopback  = { INADDR_LOOPBACK };
        const unsigned char* addr  = (const unsigned char*) &loopback;
        size_t len  = sizeof(loopback);

        inet = InetAddress::getByAddress(addr, len);
        //list = new InetAddressList(1, *inet);
        //delete inet;
        list = new InetAddressList;
        if (inet != 0)
          list->push_back(inet);

        // IPv6
        struct in6_addr loopback6 = in6addr_loopback;
        const unsigned char* addr6 = (const unsigned char*) &loopback6;
        size_t len6 = sizeof(loopback6);

        inet = InetAddress::getByAddress(addr6, len6);
        if (inet != 0)
          list->push_back(inet);

        return list;
    }


    // TODO: change to getaddrinfo

    struct addrinfo *res;
    struct addrinfo hints;
    memset(&hints, '\0', sizeof(hints));

    hints.ai_family = PF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    
    int error = getaddrinfo(host, NULL, &hints, &res);

    if (error != 0)
       throw UnknownHostException(gai_strerror(error));

    list = new InetAddressList;
    
    for (struct addrinfo *r = res; r != NULL; r = r->ai_next) {
      //mcpg - const unsigned char* addr = (const unsigned char*) (r->ai_addr);
      const unsigned char* addr=NULL;
      switch (r->ai_family) {
        case AF_INET: {
          struct sockaddr_in *sin = (sockaddr_in *)r->ai_addr;
          in_addr_t addr_t = sin->sin_addr.s_addr;
          addr = (const unsigned char *)&addr_t;
          break;
        }
        case AF_INET6: {
          struct sockaddr_in6 *sin6 = (sockaddr_in6 *)r->ai_addr;
          struct in6_addr *addr6 = &sin6->sin6_addr;
          addr = (const unsigned char *)addr6->s6_addr;
      	  break;	
        }
      }
      /**************
      if (ipv6 == false)
      {
      	if (r->ai_family == AF_INET)
      	{
      		cerr << "InetAddress::getAllByName.IPv4" << endl;
	        struct sockaddr_in *sin = (sockaddr_in *)r->ai_addr;
	        in_addr_t addr_t = sin->sin_addr.s_addr;
	        addr = (const unsigned char *)&addr_t;
        }
      }
      else
      {
      	if (r->ai_family == AF_INET6)
      	{
      		cerr << "InetAddress::getAllByName.IPv6" << endl;
        	struct sockaddr_in6 *sin6 = (sockaddr_in6 *)r->ai_addr;
	        struct in6_addr *addr6 = &sin6->sin6_addr;
	        addr = (const unsigned char *)addr6->s6_addr;
        }
      }
      **********************/
      size_t len = r->ai_addrlen;
      if (addr != 0)
      {
        try {
          inet = InetAddress::getByAddress(host, addr, len);
        } catch(UnknownHostException) {
          delete inet;
        }
      }

      if (inet != 0)
        list->push_back(inet);
      
    }

    freeaddrinfo(res);



//     // Consulta la direccion IP del host
//     struct hostent ht, *ht_ptr;
//     char           ht_buffer[HOSTENT_BUFFER_SIZE];
//     int            ht_error;

// #ifdef __sun
//     ht_ptr = gethostbyname_r(host, &ht, ht_buffer,
//                              HOSTENT_BUFFER_SIZE, &ht_error);
// #else
//     ht_ptr = gethostbyname(host);
//     ht_error = h_errno;
// #endif

//     if (ht_ptr == NULL)
//     {
//         char* errmsg = (char*) "Undefined error";
//         switch(ht_error)
//         {
//             case HOST_NOT_FOUND:
//                 errmsg = (char*) "Host not found";
//                 break;

//             case TRY_AGAIN:
//                 errmsg = (char*) "Server fail (try again)";
//                 break;

//             case NO_RECOVERY:
//                 errmsg = (char*) "Non recoverable error";
//                 break;

//             case NO_DATA:
//                 errmsg = (char*) "Valid name, no data record";
//                 break;
//         }
//         throw UnknownHostException(errmsg);
//     }

//     // Crea una lista vacia y recorre los resultados de gethostbyname()
//     list = new InetAddressList;
//     for (char** ptr = ht_ptr->h_addr_list; *ptr; ptr++)
//     {
//         try
//         {
//             const unsigned char* addr = (const unsigned char*) *ptr;
//             size_t len = ht_ptr->h_length;

//             inet = InetAddress::getByAddress(host, addr, len);
//             //list->push_back(*inet);
//             //delete inet;
//             list->push_back(inet);
//         }
//         catch(UnknownHostException)
//         {
//             delete list;
//             throw;
//         }
//     }

    // Devuelve el resultado
    return list;
}




//
// static getByAddress()
//
InetAddress* InetAddress::getByAddress(const unsigned char* addr, size_t len)
    throw(UnknownHostException)
{
    return InetAddress::getByAddress("", addr, len);
}




//
// static getByAddress()
//
InetAddress* InetAddress::getByAddress(const char* host,
                                       const unsigned char* addr, size_t len)
    throw(UnknownHostException)
{
  //mcpg - if (len == sizeof(in_addr_t)) { // IPv4
  if (len == INET_ADDRSTRLEN) { // IPv4
    
    Inet4Address* inet4 = new Inet4Address;
#ifndef __linux
    *((in_addr_t*) &(inet4->_ip)) = *((in_addr_t*) addr);
#else
    *((in_addr_t*) &(inet4->_ip.s_addr)) = *((in_addr_t*) addr);
#endif
    inet4->_hostname = host;
    return inet4;

  // mcpg - } else if ( (len > sizeof(in_addr_t)) /*&& (len <= sizeof(in6_addr))*/ ) { // IPv6
  } else if (len > INET_ADDRSTRLEN) { //IPv6
    
    Inet6Address* inet6 = new Inet6Address;
#ifndef __linux
    *((in6_addr_t*) &(inet6->_ip)) = *((in6_addr_t*) addr);
#else
    *((in6_addr*) &(inet6->_ip.s6_addr)) = *((in6_addr*) addr);
#endif
    inet6->_hostname = host;
    return inet6;

  } else {

    throw UnknownHostException("Invalid IP address");
    return 0;

  }

//     // Comprueba la longitud de la direccion
//     if (len != sizeof(in_addr_t))
//     {
//         throw UnknownHostException("Invalid IP address");
//     }

//     // Crea un nuevo objeto y lo inicializa
//     InetAddress* inet = new InetAddress;
// #ifndef __linux
//     *((in_addr_t*) &(inet->_ip)) = *((in_addr_t*) addr);
// #else
//     *((in_addr_t*) &(inet->_ip.s_addr)) = *((in_addr_t*) addr);
// #endif
//     inet->_hostname = host;

//     return inet;
}




} //namespace TIDSocket




//
// operator<<
//
ostream& operator<< (ostream& os, const TIDSocket::InetAddress& inet)
{
    size_t len;
    const unsigned char* addr = inet.getAddress(len); // TODO: need to review: may be IPv6
    
    if (len == sizeof(in_addr_t)) { // IPv4
      char buffer[INET_ADDRSTRLEN];
      os << inet_ntop(AF_INET, addr, buffer, INET_ADDRSTRLEN);
    } else if (len == sizeof(in6_addr)) { // IPv6
      char buffer[INET6_ADDRSTRLEN];
      os << inet_ntop(AF_INET6, addr, buffer, INET6_ADDRSTRLEN);
    }
    return os;
}
