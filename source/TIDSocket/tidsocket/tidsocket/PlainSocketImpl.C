///////////////////////////////////////////////////////////////////////////
//
// File:        TIDSocket_PlainSocketImpl.C
// Description: This file contains the PlainSocketImpl class methods
// Rel:         01.00
// Created:     May, 2001
// Author:      Pablo Rodriguez Archilla      ( pra@tid.es   )
//
// Revised:
//   02/29/2008 by Alvaro Polo <apv@tid.es>
//      * Some ioctl constants defined conditionally for Darwin
//        (Mac OS X) systems
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


#ifdef __sun
#include <sys/conf.h>
#include <inttypes.h>
#endif //__sun

#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#ifdef __linux
#include <net/if.h>
#endif //__linux
#if (defined __darwin || defined __CYGWIN__ || defined __ANDROID__ || defined __mips)
   #include <sys/ioctl.h>

   // I_NREAD was declared in CoreServices framework in OSX < 10.4
   // This framework cannot be used in 10.5 and later, so this
   // value is included directly
   #define I_NREAD ((65 << 8) | 1)
#else
   #include <stropts.h>
#endif

#include <unistd.h>
#include <signal.h>
#include <strings.h>
#include <fcntl.h>
#include <errno.h>

#ifdef __GNUC__
#define _fd        SocketImpl::_fd		// using SocketImpl::_fd;
#define _localport SocketImpl::_localport	// using SocketImpl::_localport;
#endif



namespace TIDSocket
{




//
// Constructor
//
PlainSocketImpl::PlainSocketImpl()
    throw()
{
    // _fd
    // _localport
    // _address
    // _port
    _timeout = 0;
    _address = NULL;

    // Captura de SIGPIPE para evitar finalizacion del proceso
    signal(SIGPIPE, SIG_IGN);
}

//
// Destructor
//
PlainSocketImpl::~PlainSocketImpl()
    throw()
{
}


//
// getOption()
//
void* PlainSocketImpl::getOption(int optID, size_t& result_size) const
    throw(SocketException)
{
    void*     result;

    int       optlevel;
    int       optname;
    void*     optval;
    socklen_t optlen;

    switch(optID)
    {
        case SocketOptions::_IP_MULTICAST_IF:
        case SocketOptions::_IP_MULTICAST_IF2:
        {
            optlevel = IPPROTO_IP;
            optname  = IP_MULTICAST_IF;

            struct in_addr addr;
            optval = (void*) &addr;
            optlen = (socklen_t) sizeof(addr);
            if (::getsockopt((int) _fd, optlevel, optname, optval, &optlen))
            {
                throw SocketException("getOption() error", errno);
            }

            result = (void*) toInetAddress(optval, optlen);
            result_size = sizeof(InetAddress);
            break;
        }
        //mcpg
        case SocketOptions::_IPV6_MULTICAST_IF:
        case SocketOptions::_IPV6_MULTICAST_IF2:
        {
            optlevel = IPPROTO_IPV6;
            optname  = IPV6_MULTICAST_IF;

            struct in_addr addr;
            optval = (void*) &addr;
            optlen = (socklen_t) sizeof(addr);
            if (::getsockopt((int) _fd, optlevel, optname, optval, &optlen))
            {
                throw SocketException("getOption() error", errno);
            }

            result = (void*) toInetAddress(optval, optlen);
            result_size = sizeof(InetAddress);
            break;
        }
        case SocketOptions::_IP_MULTICAST_LOOP:
        {
            optlevel = IPPROTO_IP;
            optname  = IP_MULTICAST_LOOP;

            unsigned char loop;
            optval = (void*) &loop;
            optlen = (socklen_t) sizeof(loop);
            if (::getsockopt((int) _fd, optlevel, optname, optval, &optlen))
            {
                throw SocketException("getOption() error", errno);
            }

            result = (void*) toBool(optval, optlen);
            result_size = sizeof(bool);
            break;
        }
        //mcpg
        case SocketOptions::_IPV6_MULTICAST_LOOP:
        {
            optlevel = IPPROTO_IPV6;
            optname  = IPV6_MULTICAST_LOOP;

            unsigned char loop;
            optval = (void*) &loop;
            optlen = (socklen_t) sizeof(loop);
            if (::getsockopt((int) _fd, optlevel, optname, optval, &optlen))
            {
                throw SocketException("getOption() error", errno);
            }

            result = (void*) toBool(optval, optlen);
            result_size = sizeof(bool);
            break;
        }
        case SocketOptions::_IP_TOS:
        {
            optlevel = IPPROTO_IP;
            optname  = IP_TOS;

            int tos;
            optval = (void*) &tos;
            optlen = (socklen_t) sizeof(tos);
            if (::getsockopt((int) _fd, optlevel, optname, optval, &optlen))
            {
                throw SocketException("getOption() error", errno);
            }

            result = (void*) toInt(optval, optlen);
            result_size = sizeof(int);
            break;
        }

        case SocketOptions::_SO_BINDADDR:
        {
            struct sockaddr addr;
            optval = (void*) &addr;
            optlen = (socklen_t) sizeof(addr);
            if (::getsockname((int) _fd, &addr, &optlen))
            {
                throw SocketException("getOption() error", errno);
            }

            result = (void*) toInetSocketAddress(optval, optlen);
            result_size = sizeof(InetSocketAddress);
            break;
        }

        case SocketOptions::_SO_BROADCAST:
        {
            optlevel = SOL_SOCKET;
            optname  = SO_BROADCAST;

            int broadcast;
            optval = (void*) &broadcast;
            optlen = (socklen_t) sizeof(broadcast);
            if (::getsockopt((int) _fd, optlevel, optname, optval, &optlen))
            {
                throw SocketException("getOption() error", errno);
            }

            result = (void*) toBool(optval, optlen);
            result_size = sizeof(bool);
            break;
        }

        case SocketOptions::_SO_KEEPALIVE:
        {
            optlevel = SOL_SOCKET;
            optname  = SO_KEEPALIVE;

            int keepalive;
            optval = (void*) &keepalive;
            optlen = (socklen_t) sizeof(keepalive);
            if (::getsockopt((int) _fd, optlevel, optname, optval, &optlen))
            {
                throw SocketException("getOption() error", errno);
            }

            result = (void*) toBool(optval, optlen);
            result_size = sizeof(bool);
            break;
        }

        case SocketOptions::_SO_LINGER:
        {
            optlevel = SOL_SOCKET;
            optname  = SO_LINGER;

            struct linger so_linger;
            optval = (void*) &so_linger;
            optlen = (socklen_t) sizeof(so_linger);
            if (::getsockopt((int) _fd, optlevel, optname, optval, &optlen))
            {
                throw SocketException("getOption() error", errno);
            }

            result = (void*) toSeconds(optval, optlen);
            result_size = sizeof(time_t);
            break;
        }

        case SocketOptions::_SO_OOBINLINE:
        {
            optlevel = SOL_SOCKET;
            optname  = SO_OOBINLINE;

            int oobinline;
            optval = (void*) &oobinline;
            optlen = (socklen_t) sizeof(oobinline);
            if (::getsockopt((int) _fd, optlevel, optname, optval, &optlen))
            {
                throw SocketException("getOption() error", errno);
            }

            result = (void*) toBool(optval, optlen);
            result_size = sizeof(bool);
            break;
        }

        case SocketOptions::_SO_RCVBUF:
        {
            optlevel = SOL_SOCKET;
            optname  = SO_RCVBUF;

            int rcvbuf;
            optval = (void*) &rcvbuf;
            optlen = (socklen_t) sizeof(rcvbuf);
            if (::getsockopt((int) _fd, optlevel, optname, optval, &optlen))
            {
                throw SocketException("getOption() error", errno);
            }

            result = (void*) toSize(optval, optlen);
            result_size = sizeof(size_t);
            break;
        }

        case SocketOptions::_SO_REUSEADDR:
        {
            optlevel = SOL_SOCKET;
            optname  = SO_REUSEADDR;

            int reuse;
            optval = (void*) &reuse;
            optlen = (socklen_t) sizeof(reuse);
            if (::getsockopt((int) _fd, optlevel, optname, optval, &optlen))
            {
                throw SocketException("getOption() error", errno);
            }

            result = (void*) toBool(optval, optlen);
            result_size = sizeof(bool);
            break;
        }

        case SocketOptions::_SO_SNDBUF:
        {
            optlevel = SOL_SOCKET;
            optname  = SO_SNDBUF;

            int sndbuf;
            optval = (void*) &sndbuf;
            optlen = (socklen_t) sizeof(sndbuf);
            if (::getsockopt((int) _fd, optlevel, optname, optval, &optlen))
            {
                throw SocketException("getOption() error", errno);
            }

            result = (void*) toSize(optval, optlen);
            result_size = sizeof(size_t);
            break;
        }

        case SocketOptions::_SO_TIMEOUT:
        {
            result_size = sizeof(time_t);
            result = (void*) new time_t;
            *((time_t*) result) = _timeout;
            break;
        }

        case SocketOptions::_TCP_NODELAY:
        {
            optlevel = IPPROTO_TCP;
            optname  = TCP_NODELAY;

            int nodelay;
            optval = (void*) &nodelay;
            optlen = (socklen_t) sizeof(nodelay);
            if (::getsockopt((int) _fd, optlevel, optname, optval, &optlen))
            {
                throw SocketException("getOption() error", errno);
            }

            result = (void*) toBool(optval, optlen);
            result_size = sizeof(bool);
            break;
        }

        default:
            throw SocketException("Unrecognized socket option");
    }

    return result;
}




//
// setOption()
//
void PlainSocketImpl::setOption(int optID, const void* value, size_t size)
    throw(SocketException)
{
    int       optlevel;
    int       optname;
    void*     optval;
    socklen_t optlen;

    switch(optID)
    {
        case SocketOptions::_IP_MULTICAST_IF:
        case SocketOptions::_IP_MULTICAST_IF2:
        {
            optlevel = IPPROTO_IP;
            optname  = IP_MULTICAST_IF;

            struct in_addr addr;
            optval = (void*) &addr;
            optlen = (socklen_t) sizeof(addr);
            fromInetAddress(value, size, optval, optlen);
            if (::setsockopt((int) _fd, optlevel, optname, optval, optlen))
            {
                throw SocketException("setOption() error", errno);
            }
            break;
        }
        
        //mcpg
        case SocketOptions::_IPV6_MULTICAST_IF:
        case SocketOptions::_IPV6_MULTICAST_IF2:
        {
            optlevel = IPPROTO_IPV6;
            optname  = IPV6_MULTICAST_IF;

            struct in_addr addr;
            optval = (void*) &addr;
            optlen = (socklen_t) sizeof(addr);
            fromInetAddress(value, size, optval, optlen);
            if (::setsockopt((int) _fd, optlevel, optname, optval, optlen))
            {
                throw SocketException("setOption() error", errno);
            }
            break;
        }

        case SocketOptions::_IP_MULTICAST_LOOP:
        {
            optlevel = IPPROTO_IP;
            optname  = IP_MULTICAST_LOOP;

            unsigned char loop;
            optval = (void*) &loop;
            optlen = (socklen_t) sizeof(loop);
            fromBool(value, size, optval, optlen);
            if (::setsockopt((int) _fd, optlevel, optname, optval, optlen))
            {
                throw SocketException("setOption() error", errno);
            }
            break;
        }
        //mcpg
        case SocketOptions::_IPV6_MULTICAST_LOOP:
        {
            optlevel = IPPROTO_IPV6;
            optname  = IPV6_MULTICAST_LOOP;

            unsigned char loop;
            optval = (void*) &loop;
            optlen = (socklen_t) sizeof(loop);
            fromBool(value, size, optval, optlen);
            if (::setsockopt((int) _fd, optlevel, optname, optval, optlen))
            {
                throw SocketException("setOption() error", errno);
            }
            break;
        }

        case SocketOptions::_IP_TOS:
        {
            optlevel = IPPROTO_IP;
            optname  = IP_TOS;

            int tos;
            optval = (void*) &tos;
            optlen = (socklen_t) sizeof(tos);
            fromInt(value, size, optval, optlen);
            if (::setsockopt((int) _fd, optlevel, optname, optval, optlen))
            {
                throw SocketException("setOption() error", errno);
            }
            break;
        }

        case SocketOptions::_SO_BINDADDR:
        {
            throw SocketException("This option cannot be set");
            break;
        }

        case SocketOptions::_SO_BROADCAST:
        {
            optlevel = SOL_SOCKET;
            optname  = SO_BROADCAST;

            int broadcast;
            optval = (void*) &broadcast;
            optlen = (socklen_t) sizeof(broadcast);
            fromBool(value, size, optval, optlen);
            if (::setsockopt((int) _fd, optlevel, optname, optval, optlen))
            {
                throw SocketException("setOption() error", errno);
            }
            break;
        }

        case SocketOptions::_SO_KEEPALIVE:
        {
            optlevel = SOL_SOCKET;
            optname  = SO_KEEPALIVE;

            int keepalive;
            optval = (void*) &keepalive;
            optlen = (socklen_t) sizeof(keepalive);
            fromBool(value, size, optval, optlen);
            if (::setsockopt((int) _fd, optlevel, optname, optval, optlen))
            {
                throw SocketException("setOption() error", errno);
            }
            break;
        }

        case SocketOptions::_SO_LINGER:
        {
            optlevel = SOL_SOCKET;
            optname  = SO_LINGER;

            struct linger so_linger;
            optval = (void*) &so_linger;
            optlen = (socklen_t) sizeof(so_linger);
            fromSeconds(value, size, optval, optlen);
            if (::setsockopt((int) _fd, optlevel, optname, optval, optlen))
            {
                throw SocketException("setOption() error", errno);
            }
            break;
        }

        case SocketOptions::_SO_OOBINLINE:
        {
            optlevel = SOL_SOCKET;
            optname  = SO_OOBINLINE;

            int oobinline;
            optval = (void*) &oobinline;
            optlen = (socklen_t) sizeof(oobinline);
            fromBool(value, size, optval, optlen);
            if (::setsockopt((int) _fd, optlevel, optname, optval, optlen))
            {
                throw SocketException("setOption() error", errno);
            }
            break;
        }

        case SocketOptions::_SO_RCVBUF:
        {
            optlevel = SOL_SOCKET;
            optname  = SO_RCVBUF;

            int rcvbuf;
            optval = (void*) &rcvbuf;
            optlen = (socklen_t) sizeof(rcvbuf);
            fromSize(value, size, optval, optlen);
            if (::setsockopt((int) _fd, optlevel, optname, optval, optlen))
            {
                throw SocketException("setOption() error", errno);
            }
            break;
        }

        case SocketOptions::_SO_REUSEADDR:
        {
            optlevel = SOL_SOCKET;
            optname  = SO_REUSEADDR;

            int reuse;
            optval = (void*) &reuse;
            optlen = (socklen_t) sizeof(reuse);
            fromBool(value, size, optval, optlen);
            if (::setsockopt((int) _fd, optlevel, optname, optval, optlen))
            {
                throw SocketException("setOption() error", errno);
            }
            break;
        }

        case SocketOptions::_SO_SNDBUF:
        {
            optlevel = SOL_SOCKET;
            optname  = SO_SNDBUF;

            int sndbuf;
            optval = (void*) &sndbuf;
            optlen = (socklen_t) sizeof(sndbuf);
            fromSize(value, size, optval, optlen);
            if (::setsockopt((int) _fd, optlevel, optname, optval, optlen))
            {
                throw SocketException("setOption() error", errno);
            }
            break;
        }

        case SocketOptions::_SO_TIMEOUT:
        {
            if (size != sizeof(time_t))
            {
                throw SocketException("Invalid value size");
            }

            _timeout = *((time_t*) value);
            break;
        }

        case SocketOptions::_TCP_NODELAY:
        {
            optlevel = IPPROTO_TCP;
            optname  = TCP_NODELAY;

            int nodelay;
            optval = (void*) &nodelay;
            optlen = (socklen_t) sizeof(nodelay);
            fromBool(value, size, optval, optlen);
            if (::setsockopt((int) _fd, optlevel, optname, optval, optlen))
            {
                throw SocketException("setOption() error", errno);
            }
            break;
        }

        default:
            throw SocketException("Unrecognized socket option");
    }
}




//
// accept()
//
void PlainSocketImpl::accept(SocketImpl* s)
    throw(IOException)
{
    // Variables auxiliares
    int flags;
    int error;
    int new_fd;

    // timeout expresado en microsegundos (por defecto, timeout infinito)
    struct timeval  tval_timeout;
    struct timeval* timeoutptr = NULL;

    // Si existe timeout, hay que poner el socket en modo no bloqueante
    if (_timeout)
    {
        // Flags actuales
        if ((flags = fcntl((int) _fd, F_GETFL)) == -1)
        {
            throw IOException("accept() error", errno);
        }

        // Establece modo no-bloqueante
        if (fcntl((int) _fd, F_SETFL, (flags|O_NONBLOCK)) == -1)
        {
            throw IOException("accept() error", errno);
        }

        // Calcula el timeout en microsegundos
        tval_timeout.tv_sec  = ((long) _timeout) / 1000L;
        tval_timeout.tv_usec = 1000L * (((long) _timeout) % 1000L);
        timeoutptr = &tval_timeout;
    }

    // Acepta una nueva conexion
    struct sockaddr sock;
    socklen_t size = (socklen_t) sizeof(sock);
    new_fd = ::accept((int) _fd, &sock, &size);
    if (new_fd == -1)
    {
        if (errno == EWOULDBLOCK)
        {
            // Comprueba con select() si hay conexiones pendientes
            fd_set rset;
            FD_ZERO(&rset);
            FD_SET((int) _fd, &rset);
            error = ::select((int) _fd + 1, &rset, NULL, NULL, timeoutptr);
            if (error <= 0 || !FD_ISSET((int) _fd, &rset))
            {
                // Corrige errno para indicar vencimiento del timeout
                errno = (error == 0) ? ETIMEDOUT : errno;
            }
            else
            {
                // Acepta ahora la conexion, pues sabemos que hay pendientes
                new_fd = ::accept((int) _fd, &sock, &size);
            }
        }
    }

    // Guarda el codigo de error 'errno' y restituye el modo del socket
    error = errno;
    if (_timeout)
    {
        fcntl((int) _fd, F_SETFL, flags);
    }

    // Comprobacion de errores
    if (new_fd == -1)
    {
        if (error == ETIMEDOUT)
            throw SocketTimeoutException("accept() timeout");
        else if (error == EBADF)
            throw SocketException("ServerSocket closed");
        else
            throw IOException("accept() error", error);
    }

    // Obtiene InetAddress con los datos de la conexion e inicializa SocketImpl
    InetSocketAddress* inet = toInetSocketAddress((void*) &sock, size);
    s->_fd        = (FileDescriptor) new_fd;
    s->_timeout   = _timeout;
    s->_localport = _localport;
    //s->_address   = inet->getAddress();
    if (s->_address)
      delete s->_address;
    s->_address   = inet->getAddress().clone();
    s->_port      = inet->getPort();
    delete inet;
}




//
// available()
//
size_t PlainSocketImpl::available()
    throw(IOException)
{
    int size = 0;
    if (ioctl((int) _fd, I_NREAD, &size))
    {
        throw IOException("available() error", errno);
    }

    return (size_t) size;
}




//
// bind()
//
void PlainSocketImpl::bind(const InetAddress& host, in_port_t port, const char* interface)
    throw(SocketException)
{
	  // Obtiene una estructura sockaddr a partir de (host,port)
    struct sockaddr_storage sock;
    socklen_t       size;
    toSockAddr(host, port, sock, size, interface);
    
    // Asocia sockaddr al socket
    if (::bind((int) _fd, (struct sockaddr*)&sock, size))
    {
        throw SocketException("bind() error", errno);
    }
    
    // Puerto local al que se asocia el socket
    if (port == ANY_PORT)
    {
        size = (socklen_t) sizeof(sock);
        if (::getsockname((int) _fd, (struct sockaddr*)&sock, &size))
        {
            throw SocketException("bind() error", errno);
        }

        port = (in_port_t) ntohs(((struct sockaddr_in*) &sock)->sin_port);
    }

    _localport = port;
}




//
// bind()
//
void PlainSocketImpl::bind(in_port_t lport, const InetAddress& laddr, const char* interface)
    throw(SocketException)
{
    bind(laddr, lport, interface);
}



//
// close()
//
void PlainSocketImpl::close()
    throw(IOException)
{
    if (::close((int) _fd))
    {
        throw IOException("close() error", errno);
    }
}




//
// connect()
//
void PlainSocketImpl::connect(const InetAddress& address, in_port_t port,const char* interface)
    throw(SocketException)
{
	  // Obtiene una estructura sockaddr a partir de (address,port)
    struct sockaddr_storage sock;
    socklen_t       size;
    toSockAddr(address, port, sock, size, interface);

    // Intenta la conexion
    if (::connect((int) _fd, (struct sockaddr*)&sock, size))
    {
        throw SocketException("connect() error", errno);
    }

    // Averigua el puerto al que a quedado ligado el socket
    size = (socklen_t) sizeof(sock);
    if (::getsockname((int) _fd, (struct sockaddr*)&sock, &size))
    {
        throw SocketException("connect() error", errno);
    }

    // localport y host:port al que se ha conectado el socket
    _localport = (in_port_t) ntohs(((struct sockaddr_in*) &sock)->sin_port);
    //_address   = address;
    if (_address) 
      delete _address;
    _address   = address.clone();
    _port      = port;
}




//
// connect()
//
void PlainSocketImpl::connect(const SocketAddress& address, time_t timeout,const char* interface)
    throw(SocketException)
{
	  // Particulariza address como referencia a un objeto InetSocketAddress
    const InetSocketAddress* addrptr =
        dynamic_cast<const InetSocketAddress*>(&address);

    if (addrptr == NULL)
    {
        throw SocketException("Invalid SocketAddress");
    }

    // Obtiene una estructura sockaddr a partir de (host,port)
    struct sockaddr_storage sock;
    socklen_t       size;
    toSockAddr(addrptr->getAddress(), addrptr->getPort(), sock, size, interface);

    // Convierte timeout (milisegundos) en una estructura timeval, siempre que
    // sea distinto de 0. Un puntero timeval* apuntara a esta estructura o bien
    // sera NULL para indicar un tiempo de espera infinito
    struct timeval  tval_timeout;
    struct timeval* timeoutptr = NULL;
    if (timeout)
    {
        tval_timeout.tv_sec  = ((long) timeout) / 1000L;
        tval_timeout.tv_usec = 1000L * (((long) timeout) % 1000L);
        timeoutptr = &tval_timeout;
    }

    int flags;
    int error;
    int soerr;

    // Obtiene los flags actuales para poner el socket en modo no bloqueante
    if ((flags = fcntl((int) _fd, F_GETFL)) == -1)
    {
        throw SocketException("connect() error", errno);
    }

    // Modo no bloqueante
    if (fcntl((int) _fd, F_SETFL, (flags|O_NONBLOCK)))
    {
        throw SocketException("connect() error", errno);
    }

    // Intenta establecer la conexion
    error = ::connect((int) _fd, (struct sockaddr*)&sock, size);
    if (error)
    {
        if (errno == EINPROGRESS)
        {
            fd_set rset;
            fd_set wset;
            FD_ZERO(&rset);
            FD_ZERO(&wset);
            FD_SET((int) _fd, &rset);
            FD_SET((int) _fd, &wset);
            error = ::select((int) _fd + 1, &rset, &wset, NULL, timeoutptr);
            if ((error <= 0) ||
                !(FD_ISSET((int) _fd, &rset) || FD_ISSET((int) _fd, &wset)))
            {
                // Corrige errno para indicar vencimiento del timeout
                errno = (error == 0) ? ETIMEDOUT : errno;
                error = -1;
            }
            else
            {
                // Consulta el estado de error del socket
                size = (socklen_t) sizeof(int);
                error= getsockopt((int) _fd,SOL_SOCKET,SO_ERROR, &soerr, &size);
                if (error == 0)
                {
                    errno = soerr;
                    error = (soerr) ? -1 : 0;
                }
            }
        }
    }

    // Restituye el modo original del socket (preserva 'errno')
    soerr = errno;
    fcntl((int) _fd, F_SETFL, flags);

    // Comprueba el resultado de la operacion
    if (error == -1)
    {
        if (soerr == ETIMEDOUT)
            throw SocketTimeoutException("connect() timeout");
        else
            throw SocketException("connect() error", soerr);
    }

    // Averigua el puerto al que a quedado ligado el socket
    size = (socklen_t) sizeof(sock);
    if (::getsockname((int) _fd, (struct sockaddr*)&sock, &size))
    {
        throw SocketException("connect() error", errno);
    }

    // localport y host:port al que se ha conectado el socket
    _localport = (in_port_t) ntohs(((struct sockaddr_in*) &sock)->sin_port);
    //_address   = addrptr->getAddress();
    if (_address)
      delete _address;
    _address   = addrptr->getAddress().clone();
    _port      = addrptr->getPort();
}




//
// connect()
//
void PlainSocketImpl::connect(const char* host, in_port_t port, const char* interface)
    throw(SocketException)
{
    try
    {
    	  // Resuelve la direccion IP del host
    	  InetSocketAddress addr(host, port);
    	  
    	  // Obtiene una estructura sockaddr a partir de (host,port)
        struct sockaddr_storage sock;
        socklen_t       size;
        toSockAddr(addr.getAddress(), addr.getPort(), sock, size, interface);

        // Intenta la conexion
        if (::connect((int) _fd, (struct sockaddr*)&sock, size))
        {
            throw SocketException("connect() error", errno);
        }
        // Averigua el puerto al que a quedado ligado el socket
        size = (socklen_t) sizeof(sock);
        if (::getsockname((int) _fd, (struct sockaddr*)&sock, &size))
        {
            throw SocketException("connect() error", errno);
        }

        // localport y host:port al que se ha conectado el socket
        _localport = (in_port_t) ntohs(((struct sockaddr_in*) &sock)->sin_port);
        //_address   = addr.getAddress();
        if (_address)
          delete _address;
        _address   = addr.getAddress().clone();
        _port      = addr.getPort();
    }
    catch(IllegalArgumentException& e)
    {
        throw SocketException(e.what());
    }
}




//
// create()
//
void PlainSocketImpl::create(bool stream, bool ipv6)
    throw(SocketException)
{
    // Crea un nuevo socket
    int type = (stream) ? SOCK_STREAM : SOCK_DGRAM;
    int s;

    if (ipv6)
      s = ::socket(AF_INET6, type, 0);
    else
      s = ::socket(AF_INET, type, 0);

    // Comprueba errores
    if (s == -1)
    {
        throw SocketException("create() error", errno);
    }

    // FileDescriptor
    _fd = (FileDescriptor) s;
}




//
// create()
//
void PlainSocketImpl::create(bool ipv6)
    throw(SocketException)
{
    create(false, ipv6);
}




//
// disconnect()
//
void PlainSocketImpl::disconnect()
    throw()
{
    // Get a sockaddr struct for the null address
    struct sockaddr sock;
    socklen_t       size;
    toNullSockAddr(sock, size);

    // Try disconnection and update local port number
    if (::connect((int) _fd, &sock, size) == 0)
    {
        _localport = ANY_PORT;
    }
}




//
// getFileDescriptor()
//
FileDescriptor PlainSocketImpl::getFileDescriptor() const
    throw()
{
    return _fd;
}




//
// getInputStream()
//
InputStream* PlainSocketImpl::getInputStream()
    throw(IOException)
{
    // Crea un nuevo stream
    InputStream* stream = new InputStream((int) _fd, _timeout);

    /* caceres@tid.es 14/7/2004
    // Comprueba errores
    if (stream->fail())
    {
        throw IOException("InputStream creation failed");
    }
    */

    return stream;
}




//
// getLocalPort()
//
in_port_t PlainSocketImpl::getLocalPort() const
    throw()
{
    return _localport;
}




//
// getOutputStream()
//
OutputStream* PlainSocketImpl::getOutputStream()
    throw(IOException)
{
    // Crea un nuevo stream
    OutputStream* stream = new OutputStream((int) _fd, _timeout);

    // Comprueba errores
    /* caceres@tid.es 15/7/2004
    if (stream->fail())
    {
        throw IOException("OutputStream creation failed");
    }

    // Flush automatico en cada escritura
    stream->setf(ios::unitbuf);
    */

    return stream;
}




//
// getTimeToLive()
//
int PlainSocketImpl::getTimeToLive()
    throw(IOException)
{
    unsigned char ttl;

    int       optlevel = IPPROTO_IP;
    int       optname  = IP_MULTICAST_TTL;
    void*     optval   = (void*) &ttl;
    socklen_t optlen   = (socklen_t) sizeof(ttl);

    if (::getsockopt((int) _fd, optlevel, optname, optval, &optlen))
    {
        throw IOException("getTimeToLive() error", errno);
    }

    return (int) ttl;
}




//
// join()
//
void PlainSocketImpl::join(const InetAddress& inetaddr)
    throw(IOException)
{
    NetworkInterface* netIf = NULL;

    try
    {
        // Get SocketAdress object from multicast address
        InetSocketAddress mcastaddr(&inetaddr, ANY_PORT);

        // Get NetworkInterface object with wildcard address
        //InetAddress any;
        Inet4Address any; // TODO
        netIf = NetworkInterface::getByInetAddress(any);

        joinGroup(mcastaddr, *netIf);

        delete netIf;
    }
    catch (IllegalArgumentException& e)
    {
        delete netIf;
        throw IOException(e.what());
    }
    catch (IOException& e)
    {
        delete netIf;
        throw;
    }
}




//
// joinGroup()
//
void PlainSocketImpl::joinGroup(const SocketAddress& mcastaddr,
                                const NetworkInterface& netIf)
    throw(IOException)
{
    struct ip_mreq req;
    toMulticastReq(mcastaddr, netIf, req);

    int       optlevel = IPPROTO_IP;
    int       optname  = IP_ADD_MEMBERSHIP;
    void*     optval   = (void*) &req;
    socklen_t optlen   = (socklen_t) sizeof(req);

    if (::setsockopt((int) _fd, optlevel, optname, optval, optlen))
    {
        throw IOException("joinGroup() error", errno);
    }
}




//
// leave()
//
void PlainSocketImpl::leave(const InetAddress& inetaddr)
    throw(IOException)
{
    NetworkInterface* netIf = NULL;

    try
    {
        // Get SocketAdress object from multicast address
        InetSocketAddress mcastaddr(&inetaddr, ANY_PORT);

        // Get NetworkInterface object with wildcard address
        //InetAddress any;
        Inet4Address any; // TODO
        netIf = NetworkInterface::getByInetAddress(any);

        leaveGroup(mcastaddr, *netIf);

        delete netIf;
    }
    catch (IllegalArgumentException& e)
    {
        delete netIf;
        throw IOException(e.what());
    }
    catch (IOException& e)
    {
        delete netIf;
        throw;
    }
}




//
// leaveGroup()
//
void PlainSocketImpl::leaveGroup(const SocketAddress& mcastaddr,
                                 const NetworkInterface& netIf)
    throw(IOException)
{
    struct ip_mreq req;
    toMulticastReq(mcastaddr, netIf, req);

    int       optlevel = IPPROTO_IP;
    int       optname  = IP_DROP_MEMBERSHIP;
    void*     optval   = (void*) &req;
    socklen_t optlen   = (socklen_t) sizeof(req);

    if (::setsockopt((int) _fd, optlevel, optname, optval, optlen))
    {
        throw IOException("leaveGroup() error", errno);
    }
}




//
// listen()
//
void PlainSocketImpl::listen(int backlog)
    throw(IOException)
{
    if (::listen((int) _fd, backlog))
    {
        throw IOException("listen() error", errno);
    }
}




//
// peek()
//
int PlainSocketImpl::peek(const InetAddress& i)
    throw(IOException, PortUnreachableException)
{
    throw IOException("peek() not implemented");
}




//
// peekData()
//
int PlainSocketImpl::peekData(const DatagramPacket& i)
    throw(IOException, PortUnreachableException)
{
    throw IOException("peekData() not implemented");
}




//
// receive()
//
void PlainSocketImpl::receive(DatagramPacket& i)
    throw(IOException, PortUnreachableException)
{
    struct sockaddr from;
    socklen_t       fromlen = sizeof(from);

    // Buffer to receive data
    unsigned char* buf = (unsigned char*) i.getData();
    buf += i.getOffset();

    // Receive data
    size_t length = i.getLength();
    ssize_t nread = ::recvfrom((int) _fd, buf, length, 0, &from, &fromlen);
    if (nread == -1)
    {
        throw IOException("receive() error", errno);
    }

    // Update datagram packet
    InetSocketAddress* sock = NULL;
    try
    {
        sock = toInetSocketAddress(&from, fromlen);
        i.setSocketAddress(*sock);
        i.setLength(nread);
    }
    catch (IllegalArgumentException& e)
    {
        delete sock;
        throw IOException(e.what());
    }
    delete sock;
}




//
// send()
//
void PlainSocketImpl::send(DatagramPacket& i,const char* interface)
    throw(IOException, PortUnreachableException)
{
	  struct sockaddr_storage to;
    socklen_t       tolen;

    // Check destination
    int port = i.getPort();
    if (port == -1)
    {
        throw PortUnreachableException("send() error");
    }

    // Get struct sockaddr from datagram packet
    const InetAddress* inet = i.getAddress();
    toSockAddr(*inet, port, to, tolen, interface);
    // Data buffer
    unsigned char* buf = (unsigned char*) i.getData();
    buf += i.getOffset();

    // Send data
    size_t length = i.getLength();
    ssize_t nwrite = ::sendto((int) _fd, buf, length, 0, (struct sockaddr*)&to, tolen);
    if (nwrite == -1)
    {
        throw IOException("send() error", errno);
    }
    // Update datagram packet
    try
    {
        i.setLength(nwrite);
    }
    catch (IllegalArgumentException& e)
    {
        throw IOException(e.what());
    }
}




//
// sendUrgentData()
//
void PlainSocketImpl::sendUrgentData(int data)
    throw(IOException)
{
    // Comprueba si es posible enviar datos
    if (! supportsUrgentData())
    {
        throw IOException("Sending urgent data not supported");
    }

    // Envia un byte
    unsigned char byte_data = (unsigned char) data;
    if (::send((int) _fd, (const void*) &byte_data, 1, MSG_OOB))
    {
        throw IOException("sendUrgentData() error", errno);
    }
}




//
// setTimeToLive()
//
void PlainSocketImpl::setTimeToLive(int ttl)
    throw(IOException)
{
    unsigned char uchar_ttl = (unsigned char) ttl;

    int       optlevel = IPPROTO_IP;
    int       optname  = IP_MULTICAST_TTL;
    void*     optval   = (void*) &uchar_ttl;
    socklen_t optlen   = (socklen_t) sizeof(uchar_ttl);

    if (::setsockopt((int) _fd, optlevel, optname, optval, optlen))
    {
        throw IOException("setTimeToLive() error", errno);
    }
}




//
// supportsUrgentData()
//
bool PlainSocketImpl::supportsUrgentData()
    throw()
{
    int       type;
    socklen_t size = (int) sizeof(type);

    // Comprueba el tipo de socket
    if (::getsockopt((int) _fd, SOL_SOCKET, SO_TYPE, (void*) &type, &size))
    {
        return false;
    }

    // Actualmente solo los sockets de tipo STREAM pueden enviar datos urgentes
    return (type == SOCK_STREAM);
}




//
// static getIntOption()
//
int PlainSocketImpl::getIntOption(const SocketOptions* socket, int optID)
    throw(SocketException)
{
    int result;

    switch(optID)
    {
        case SocketOptions::_IP_TOS:
        {
            size_t size;
            int* value = (int*) socket->getOption(optID, size);
            result = *value;
            delete value;
            break;
        }
        default:
        {
            throw SocketException("This socket option has no integer value");
        }
    }

    return result;
}




//
// static getBoolOption()
//
bool PlainSocketImpl::getBoolOption(const SocketOptions* socket, int optID)
    throw(SocketException)
{
    bool result;

    switch(optID)
    {
        case SocketOptions::_IP_MULTICAST_LOOP:
        case SocketOptions::_IPV6_MULTICAST_LOOP:
	case SocketOptions::_SO_BROADCAST:
	case SocketOptions::_SO_KEEPALIVE:
	case SocketOptions::_SO_OOBINLINE:
	case SocketOptions::_SO_REUSEADDR:
	case SocketOptions::_TCP_NODELAY:
        {
            size_t size;
            bool* value = (bool*) socket->getOption(optID, size);
            result = *value;
            delete value;
            break;
	}
	default:
	{
            throw SocketException("This socket option has no boolean value");
        }
    }

    return result;
}




//
// static getSizeOption()
//
size_t PlainSocketImpl::getSizeOption(const SocketOptions* socket, int optID)
    throw(SocketException)
{
    size_t result;

    switch(optID)
    {
        case SocketOptions::_SO_RCVBUF:
        case SocketOptions::_SO_SNDBUF:
        {
            size_t size;
            size_t* value = (size_t*) socket->getOption(optID, size);
            result = *value;
            delete value;
            break;
	}
	default:
	{
            throw SocketException("This socket option has no size_t value");
        }
    }

    return result;
}




//
// static getTimeOption()
//
time_t PlainSocketImpl::getTimeOption(const SocketOptions* socket, int optID)
    throw(SocketException)
{
    time_t result;

    switch(optID)
    {
        case SocketOptions::_SO_LINGER:
        case SocketOptions::_SO_TIMEOUT:
        {
            size_t size;
            time_t* value = (time_t*) socket->getOption(optID, size);
            result = *value;
            delete value;
            break;
	}
	default:
	{
            throw SocketException("This socket option has no time_t value");
        }
    }

    return result;
}




//
// static setIntOption()
//
void PlainSocketImpl::setIntOption(SocketOptions* socket, int optID, int value)
    throw(SocketException)
{
    size_t size = sizeof(value);
    socket->setOption(optID, (const void*) &value, size);
}




//
// static setBoolOption()
//
void PlainSocketImpl::setBoolOption(SocketOptions* socket, int optID, bool value)
    throw(SocketException)
{
    size_t size = sizeof(value);
    socket->setOption(optID, (const void*) &value, size);
}




//
// static setSizeOption()
//
void PlainSocketImpl::setSizeOption(SocketOptions* socket, int optID, size_t value)
    throw(SocketException)
{
    size_t size = sizeof(value);
    socket->setOption(optID, (const void*) &value, size);
}




//
// static setTimeOption()
//
void PlainSocketImpl::setTimeOption(SocketOptions* socket, int optID, time_t value)
    throw(SocketException)
{
    size_t size = sizeof(value);
    socket->setOption(optID, (const void*) &value, size);
}




//
// static toInt()
//
int* PlainSocketImpl::toInt(const void* buffer, socklen_t bufflen)
    throw()
{
    int* result = new int;

    *result = *((int*) buffer);
    return result;
}




//
// static toBool()
//
bool* PlainSocketImpl::toBool(const void* buffer, socklen_t bufflen)
    throw()
{
    unsigned char uchar_value;
    int           int_value;

    bool* result = new bool;

    if (bufflen == sizeof(uchar_value))
    {
        uchar_value = *((unsigned char*) buffer);
        *result = (uchar_value != 0);
    }
    else
    {
        int_value = *((int*) buffer);
        *result = (int_value != 0);
    }

    return result;
}




//
// static toSize()
//
size_t* PlainSocketImpl::toSize(const void* buffer, socklen_t bufflen)
    throw()
{
    size_t* result = new size_t;

    *result = (size_t) *((int*) buffer);
    return result;
}




//
// static toSeconds()
//
time_t* PlainSocketImpl::toSeconds(const void* buffer, socklen_t bufflen)
    throw()
{
    time_t* result = new time_t;

    struct linger* so_linger = (struct linger*) buffer;
    *result = (so_linger->l_onoff == 0) ? -1 : (time_t) so_linger->l_linger;

    return result;
}




//
// static toInetAddress()
//
InetAddress* PlainSocketImpl::toInetAddress(const void* buffer,
                                            socklen_t   bufflen)
    throw(SocketException)
{
    const unsigned char* addr = (const unsigned char*) buffer;
    size_t               len  = (size_t) bufflen;

    InetAddress* inet = NULL;
    try
    {
        inet = InetAddress::getByAddress(addr, len);
    }
    catch (UnknownHostException& e)
    {
        throw SocketException(e.what());
    }

    return inet;
}




//
// static toInetSocketAddress()
//
InetSocketAddress* PlainSocketImpl::toInetSocketAddress(const void* buffer,
                                                        socklen_t   bufflen)
    throw(SocketException)
{
    //mcpg - in_port_t port = (in_port_t)ntohs(((struct sockaddr_in*) buffer)->sin_port);
    //mcpg - in_addr_t addr = ((struct sockaddr_in*) buffer)->sin_addr.s_addr;
    //mcpg - size_t    len  = sizeof(addr);

    InetAddress*       inet = NULL;
    InetSocketAddress* sock = NULL;
    const unsigned char *addr;
    in_port_t port;
    size_t    len;
    try
    {
    	if (bufflen == sizeof(struct sockaddr_in))
      {
        //inet = InetAddress::getByAddress((const unsigned char*) &addr, len);
        struct sockaddr_in *sin = (sockaddr_in *)buffer;
        if (sin)
        {
        	in_addr_t addr_t = sin->sin_addr.s_addr;
        	addr = (const unsigned char *)&addr_t;
	        port = sin->sin_port;
	        len = 16;
        }
      }
      else
      {
      	struct sockaddr_in6 *sin6 = (sockaddr_in6 *)buffer;
       	if (sin6)
       	{
       		struct in6_addr *addr6 = &sin6->sin6_addr;
	         addr = (const unsigned char *)addr6->s6_addr;
	         port = sin6->sin6_port;
	         len = 48;
        }
      }
      inet = InetAddress::getByAddress(addr, len);
      sock = new InetSocketAddress(inet, port);
      delete inet;
    }
    catch (UnknownHostException& e)
    {
        delete inet;
        throw SocketException(e.what());
    }
    catch (IllegalArgumentException& e)
    {
        delete inet;
        throw SocketException(e.what());
    }

    return sock;
}




//
// static fromInt()
//
void PlainSocketImpl::fromInt(const void* buffer, size_t bufflen,
                              void*       dest,   size_t destlen)
    throw(SocketException)
{
    if (bufflen != sizeof(int))
    {
        throw SocketException("Invalid value size");
    }

    *((int*) dest) = *((int*) buffer);
}




//
// static fromBool()
//
void PlainSocketImpl::fromBool(const void* buffer, size_t bufflen,
                               void*       dest,   size_t destlen)
    throw(SocketException)
{
    if (bufflen != sizeof(bool))
    {
        throw SocketException("Invalid value size");
    }

    bool enable = *((bool*) buffer);

    if (destlen == sizeof(unsigned char))
        *((unsigned char*) dest) = (enable) ? 1 : 0;
    else
        *((int*) dest) = (enable) ? 1 : 0;
}




//
// static fromSize()
//
void PlainSocketImpl::fromSize(const void* buffer, size_t bufflen,
                               void*       dest,   size_t destlen)
    throw(SocketException)
{
    if (bufflen != sizeof(size_t))
    {
        throw SocketException("Invalid value size");
    }

    *((int*) dest) = (int) *((size_t*) buffer);
}




//
// static fromSeconds()
//
void PlainSocketImpl::fromSeconds(const void* buffer, size_t bufflen,
                                  void*       dest,   size_t destlen)
    throw(SocketException)
{
    if (bufflen != sizeof(time_t))
    {
        throw SocketException("Invalid value size");
    }

    time_t         time_value = *((time_t*) buffer);
    struct linger* so_linger  = (struct linger*) dest;

    so_linger->l_onoff  = (time_value == -1) ? 0 : 1;
    so_linger->l_linger = time_value;
}





//
// static fromInetAddress()
//
void PlainSocketImpl::fromInetAddress(const void* buffer, size_t bufflen,
                                      void*       dest,   size_t destlen)
    throw(SocketException)
{
    if (bufflen != sizeof(InetAddress))
    {
        throw SocketException("Invalid value size");
    }

    InetAddress* inet = (InetAddress*) buffer;

    size_t len;
    const void* addr = inet->getAddress(len);
    
    bcopy(addr, dest, len);
}




//
// static toMulticastReq()
//
void PlainSocketImpl::toMulticastReq(const SocketAddress&    addr,
                                     const NetworkInterface& netIf,
                                     struct ip_mreq&         req)
    throw(IOException)
{
    size_t len;

    // Particulariza address como referencia a un objeto InetSocketAddress
    const InetSocketAddress* addrptr =
        dynamic_cast<const InetSocketAddress*>(&addr);

    if (addrptr == NULL)
    {
        throw IOException("Invalid SocketAddress");
    }

    // Direcciones IP del grupo multicast y de la interfaz local de recepcion
    const InetAddressList& list = netIf.getInetAddresses();
    //in_addr_t local = *((in_addr_t*) (list[0].getAddress(len)));
    in_addr_t local = *((in_addr_t*) (list[0]->getAddress(len)));
    in_addr_t mcast = *((in_addr_t*) addrptr->getAddress().getAddress(len));

    // Inicializacion de ip_mreq
    req.imr_multiaddr.s_addr = mcast;
    req.imr_interface.s_addr = local;
}




//
// static toSockAddr()
//
void PlainSocketImpl::toSockAddr(const InetAddress& inet,
                                 in_port_t          port,
                                 struct sockaddr_storage& sock,
                                 socklen_t&         size,
                                 const char*        interface)
    throw()
{
	  // Obtiene la direccion de "inet" en formato network order
    size_t len;
    void* addr = (void*) inet.getAddress(len);
    
    if (len == sizeof(in_addr_t) ) { // IPv4
      
      // Completa la estructura sockaddr
      size = (socklen_t) sizeof(sockaddr_in);
      bzero((void*) &sock, size);
      bcopy((const void*) addr, &(((sockaddr_in*) &sock)->sin_addr), len);
      ((sockaddr_in*) &sock)->sin_family = AF_INET;
      ((sockaddr_in*) &sock)->sin_port   = htons(port);
 
    } else { // IPv6    // Obtiene la direccion de "inet" en formato network order
    	
      // Completa la estructura sockaddr
      size = (socklen_t) sizeof(sockaddr_in6);
      bzero((void*) &sock, size);
      //Para que al incluir el parametro  -ORB_ip_address el servidor no de una excepcion
      bcopy((const void*) addr, &(((sockaddr_in6*) &sock)->sin6_addr), len);    // Obtiene la direccion de "inet" en formato network order
      
      //memcpy((void*)(&((sockaddr_in6*)&sock)->sin6_addr), (const void*)addr, sizeof(struct in6_addr));
      //inet_pton(AF_INET6, (const char*)addr, &((sockaddr_in6*) &sock)->sin6_addr);
      ((sockaddr_in6*) &sock)->sin6_family = AF_INET6;
      ((sockaddr_in6*) &sock)->sin6_port   = htons(port);
#ifdef __linux
      if (interface != NULL)
        ((sockaddr_in6*) &sock)->sin6_scope_id = if_nametoindex(interface);
#endif //__linux
    }
}




//
// static toNullSockAddr()
//
void PlainSocketImpl::toNullSockAddr(struct sockaddr& sock,
                                     socklen_t&       size)
    throw()
{
    size = (socklen_t) sizeof(sockaddr);
    bzero((void*) &sock, size);
    sock.sa_family = AF_UNSPEC;
}




} //namespace TIDSocket
