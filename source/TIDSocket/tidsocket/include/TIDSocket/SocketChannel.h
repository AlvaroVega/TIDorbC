#include "TIDSocket.h"


#ifndef __TID_SOCKET_SOCKET_CHANNEL_H__
#define __TID_SOCKET_SOCKET_CHANNEL_H__


#include <sys/types.h>
#include <sys/socket.h>


using TIDThr::RecursiveMutex;
using TIDThr::SystemException;


namespace TIDSocket
{


class Socket;


class SocketChannel : public virtual SelectableChannel
{
    friend class ServerSocketChannel;

    public:
        // Destructor
        virtual ~SocketChannel()
            throw(SystemException);

        // Connects this channel's socket
        bool connect(const SocketAddress& remote,const char* interface)
            throw(AlreadyConnectedException, ConnectionPendingException,
                  ClosedChannelException, AsynchronousCloseException,
                  ClosedByInterruptException, UnresolvedAddressException,
                  UnsupportedAddressTypeException, IOException);

        // Finishes the process of connecting a socket channel
        bool finishConnect(const char* interface)
            throw(NoConnectionPendingException,
                  ClosedChannelException, AsynchronousCloseException,
                  ClosedByInterruptException, IOException);

        // Tells whether or not this channel's network socket is connected
        bool isConnected()
            throw();

        // Tells whether or not a connection operation is in progress on this
        // channel
        bool isConnectionPending()
            throw();

        // Reads a sequence of bytes from this channel into the given buffer
        ssize_t read(unsigned char* dst, size_t dst_len)
            throw(NotYetConnectedException, IOException);

        // Retrieves a socket associated with this channel
        const Socket* socket()
            throw();

        // Returns an operation set identifying this channel's supported
        // operations
        int validOps()
            throw();

        // Writes a sequence of bytes to this channel from the given buffer
        ssize_t write(const unsigned char* src, size_t src_len)
            throw(NotYetConnectedException, ClosedChannelException,
                  IOException);

    public:
        // Opens a socket channel
        // (caller must delete SocketChannel object)
        static SocketChannel* open()
            throw(IOException);

        // Opens a socket channel and connects it to a remote address
        // (caller must delete SocketChannel object)
        static SocketChannel* open(const SocketAddress& remote,const char* interface)
            throw(AsynchronousCloseException, ClosedByInterruptException,
                  UnresolvedAddressException, UnsupportedAddressTypeException,
                  IOException);

    protected:
        // Constructor
        SocketChannel()
            throw(SystemException);

        // Adjusts this channel's blocking mode
        void implConfigureBlocking(bool block)
            throw(IOException);

        // Closes this channel
        void implCloseChannel()
            throw(IOException);

        // Registers this channel with the given selector, returning a
        SelectionKey& implRegisterChannel(Selector& sel, int ops)
            throw();

    protected:
        Socket*          _socket;
        InputStream*     _istream;
        OutputStream*    _ostream;
        RecursiveMutex   _connect;

    private:
        // Pending connection address
        struct sockaddr_storage _sockaddr;
        socklen_t        _socksize;
};


} //namespace TIDSocket


#endif //__TID_SOCKET_SOCKET_CHANNEL_H__
