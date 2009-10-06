///////////////////////////////////////////////////////////////////////////
//
// File:        TIDSocket_TIDSocket.h
// Description: This file contains the TIDSocket includes
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

#ifndef __TID_SOCKET_H__
#define __TID_SOCKET_H__


#include "TIDThr.h"

#include "TIDSocket/config.h"
#include "TIDSocket/Exception.h"
#include "TIDSocket/IOException.h"
#include "TIDSocket/AlreadyConnectedException.h"
#include "TIDSocket/ConnectionPendingException.h"
#include "TIDSocket/ClosedChannelException.h"
#include "TIDSocket/AsynchronousCloseException.h"
#include "TIDSocket/ClosedByInterruptException.h"
#include "TIDSocket/IllegalArgumentException.h"
#include "TIDSocket/IllegalBlockingModeException.h"
#include "TIDSocket/IndexOutOfBoundsException.h"
#include "TIDSocket/NoConnectionPendingException.h"
#include "TIDSocket/NotYetBoundException.h"
#include "TIDSocket/NotYetConnectedException.h"
#include "TIDSocket/SocketException.h"
#include "TIDSocket/SocketTimeoutException.h"
#include "TIDSocket/PortUnreachableException.h"
#include "TIDSocket/UnknownHostException.h"
#include "TIDSocket/UnresolvedAddressException.h"
#include "TIDSocket/UnsupportedAddressTypeException.h"
#include "TIDSocket/FileDescriptor.h"
#include "TIDSocket/SocketAddress.h"
#include "TIDSocket/InetAddress.h"
#include "TIDSocket/InetSocketAddress.h"
#include "TIDSocket/NetworkInterface.h"
#include "TIDSocket/SelectionKey.h"
#include "TIDSocket/SelectableChannel.h"
#include "TIDSocket/Selector.h"
#include "TIDSocket/InputStream.h"
#include "TIDSocket/OutputStream.h"
#include "TIDSocket/ServerSocketChannel.h"
#include "TIDSocket/SocketOptions.h"
#include "TIDSocket/SocketChannel.h"
#include "TIDSocket/SocketImpl.h"
#include "TIDSocket/SocketImplFactory.h"
#include "TIDSocket/Socket.h"
#include "TIDSocket/ServerSocket.h"
#include "TIDSocket/DatagramChannel.h"
#include "TIDSocket/DatagramPacket.h"
#include "TIDSocket/DatagramSocketImpl.h"
#include "TIDSocket/DatagramSocketImplFactory.h"
#include "TIDSocket/DatagramSocket.h"
#include "TIDSocket/MulticastSocket.h"


namespace TIDSocket
{


typedef TIDThr::HandleT<Socket>            Socket_ref;
typedef TIDThr::HandleT<ServerSocket>      ServerSocket_ref;
typedef TIDThr::HandleT<DatagramSocket>    DatagramSocket_ref;
typedef TIDThr::HandleT<MulticastSocket>   MulticastSocket_ref;


} //namespace TIDSocket


#endif //__TID_SOCKET_H__
