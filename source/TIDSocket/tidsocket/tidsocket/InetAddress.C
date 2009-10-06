///////////////////////////////////////////////////////////////////////////
//
// File:        TIDSocket_InetAddress.C
// Description: This file contains the InetAddress class methods
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
InetAddress::InetAddress()
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
InetAddress::~InetAddress()
    throw()
{
}




//
// Assign operator
//
InetAddress& InetAddress::operator= (const InetAddress& addr)
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
bool InetAddress::operator== (const InetAddress& addr)
    throw()
{
#ifndef __linux
    return ((*((in_addr_t*) &_ip) == *((in_addr_t*) &(addr._ip)))
#else
    return ((*((in_addr_t*) &(_ip.s_addr)) == *((in_addr_t*) &(addr._ip.s_addr)))
#endif
            && (_hostname == addr._hostname));
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
// getAddress()
//
const unsigned char* InetAddress::getAddress(size_t& out_len) const
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
// getHostAddress()
//
char* InetAddress::getHostAddress()
    throw()
{
    // Obtiene la representacion textual de la direccion IP
    char buffer[INET_ADDRSTRLEN];
    string addr = inet_ntop(AF_INET, &_ip, buffer, INET_ADDRSTRLEN);

    // Devuelve una copia
    return duplicate(addr);
}




//
// getCanonicalHostName()
//
char* InetAddress::getCanonicalHostName()
    throw()
{
    // Obtiene un puntero a la direccion IP
    size_t len;
    const unsigned char* ip_ptr = getAddress(len);

    // Consulta el nombre del host a partir de la direccion IP; en caso de error
    // devuelve "" (ya que este metodo no genera ninguna excepcion a tal efecto)
    struct hostent ht, *ht_ptr;
    char           ht_buffer[HOSTENT_BUFFER_SIZE];
    int            ht_error;

#ifdef __sun
    ht_ptr = gethostbyaddr_r((const char*) ip_ptr, len, AF_INET,
                             &ht, ht_buffer, HOSTENT_BUFFER_SIZE, &ht_error);
#else
    ht_ptr = gethostbyaddr((const void*) ip_ptr, len, AF_INET);
    ht_error = h_errno;
#endif

    string canonical = (ht_ptr) ? (const char*) ht_ptr->h_name : "";

    // Devuelve una copia
    return duplicate(canonical);
}




//
// isAnyLocalAddress()
//
bool InetAddress::isAnyLocalAddress() const
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
// isLinkLocalAddress()
//
bool InetAddress::isLinkLocalAddress() const
    throw()
{
    return false;
}




//
// isLoopbackAddress()
//
bool InetAddress::isLoopbackAddress() const
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
bool InetAddress::isMCGlobal() const
    throw()
{
    return false;
}




//
// isMCLinkLocal()
//
bool InetAddress::isMCLinkLocal() const
    throw()
{
    return false;
}




//
// isMCNodeLocal()
//
bool InetAddress::isMCNodeLocal() const
    throw()
{
    return false;
}




//
// isMCOrgLocal()
//
bool InetAddress::isMCOrgLocal() const
    throw()
{
    return false;
}




//
// isMCSiteLocal()
//
bool InetAddress::isMCSiteLocal() const
    throw()
{
    return false;
}




//
// isMulticastAddress()
//
bool InetAddress::isMulticastAddress() const
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
bool InetAddress::isSiteLocalAddress() const
    throw()
{
    return false;
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
    if (list->size() == 0)
    {
        delete list;
        throw UnknownHostException("getByName() error");
    }

    // Copia la primera dirección
    inet = new InetAddress(list->at(0));
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

    // Si host es NULL, devuelve la dirección de bucle local
    if (host == NULL)
    {
        struct in_addr loopback = { INADDR_LOOPBACK };
        const unsigned char* addr = (const unsigned char*) &loopback;
        size_t len = sizeof(loopback);

        inet = InetAddress::getByAddress(addr, len);
        list = new InetAddressList(1, *inet);
        delete inet;

        return list;
    }

    // Consulta la direccion IP del host
    struct hostent ht, *ht_ptr;
    char           ht_buffer[HOSTENT_BUFFER_SIZE];
    int            ht_error;

#ifdef __sun
    ht_ptr = gethostbyname_r(host, &ht, ht_buffer,
                             HOSTENT_BUFFER_SIZE, &ht_error);
#else
    ht_ptr = gethostbyname(host);
    ht_error = h_errno;
#endif

    if (ht_ptr == NULL)
    {
        char* errmsg = (char*) "Undefined error";
        switch(ht_error)
        {
            case HOST_NOT_FOUND:
                errmsg = (char*) "Host not found";
                break;

            case TRY_AGAIN:
                errmsg = (char*) "Server fail (try again)";
                break;

            case NO_RECOVERY:
                errmsg = (char*) "Non recoverable error";
                break;

            case NO_DATA:
                errmsg = (char*) "Valid name, no data record";
                break;
        }
        throw UnknownHostException(errmsg);
    }

    // Crea una lista vacia y recorre los resultados de gethostbyname()
    list = new InetAddressList;
    for (char** ptr = ht_ptr->h_addr_list; *ptr; ptr++)
    {
        try
        {
            const unsigned char* addr = (const unsigned char*) *ptr;
            size_t len = ht_ptr->h_length;

            inet = InetAddress::getByAddress(host, addr, len);
            list->push_back(*inet);
            delete inet;
        }
        catch(UnknownHostException)
        {
            delete list;
            throw;
        }
    }

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
    // Comprueba la longitud de la direccion
    if (len != sizeof(in_addr_t))
    {
        throw UnknownHostException("Invalid IP address");
    }

    // Crea un nuevo objeto y lo inicializa
    InetAddress* inet = new InetAddress;
#ifndef __linux
    *((in_addr_t*) &(inet->_ip)) = *((in_addr_t*) addr);
#else
    *((in_addr_t*) &(inet->_ip.s_addr)) = *((in_addr_t*) addr);
#endif
    inet->_hostname = host;

    return inet;
}




} //namespace TIDSocket




//
// operator<<
//
ostream& operator<< (ostream& os, const TIDSocket::InetAddress& inet)
{
    size_t len;
    const unsigned char* addr = inet.getAddress(len);

    char buffer[INET_ADDRSTRLEN];
    os << inet_ntop(AF_INET, addr, buffer, INET_ADDRSTRLEN);
    return os;
}
