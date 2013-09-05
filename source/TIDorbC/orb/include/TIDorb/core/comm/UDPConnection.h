/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/UDPConnection.h
  
 Revisions:
 
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
 ------------------------------------------------------------------------------ */

#include "TIDorb/core/comm.h"

#ifndef _TIDORB_core_comm_UDPConnection_H_
#define _TIDORB_core_comm_UDPConnection_H_ 1


/**
 * TIDorb UDP connection: sends and receives MIOP unicast messages.
 */

#include <sys/types.h>
#include "TIDSocket.h"

namespace TIDorb {
namespace core {
namespace comm {

class UDPConnection : public Connection
{
  protected:
    /**
     * Connection socket.
     */
    TIDSocket::DatagramSocket* socket;

    /**
     * Buffer for sending MIOP packet collections.
     */
    TIDorb::core::cdr::BufferCDR_ref miop_packet_buffer;

    /**
     * Buffer for receiving datagrams.
     */
    TIDorb::core::cdr::BufferCDR_ref receive_miop_header_buffer;
    unsigned char* datagram_buffer;
    size_t datagram_max_size;
    size_t datagram_size;
    size_t datagram_offset;


    /**
     * UseTable for compose MIOP PacketCollection
     */
    typedef TIDorb::core::util::UseTable<TIDorb::core::comm::miop::UniqueId, 
                                         TIDorb::core::comm::miop::MIOPPacketCollection> 
      MIOPPacketCollectionTable;
    MIOPPacketCollectionTable collections;

    /**
     * Register the connection at the connection manager.
     */
    UDPConnection(ConnectionManager* mngr, TIDSocket::DatagramSocket* s)
      throw (TIDThr::SystemException, CORBA::SystemException);

    /**
     * Delay to avoid datagram corruption
     */
    struct timespec delay;

  public:

    virtual ~UDPConnection() throw (TIDThr::SystemException);

    /**
     * Connection hash code.
     */
    CORBA::ULongLong hashCode();

    const char* toString();

    void close();

    void dispatch_request(TIDorb::core::comm::iiop::GIOPLocateRequestMessage* message);
    void dispatch_request(TIDorb::core::comm::iiop::GIOPRequestMessage* message);

    void send_oneway_request_async(TIDorb::core::RequestImpl* request,
                                   TIDorb::core::iop::IOR* ior)
      throw (RECOVERABLE_COMM_FAILURE,CORBA::SystemException);

    void send_oneway_request_async(TIDorb::core::RequestImpl* request,
                                   TIDorb::core::iop::IOR* ior,
                                   TIDorb::core::comm::iiop::AddressingDisposition disposition)
      throw (RECOVERABLE_COMM_FAILURE,CORBA::SystemException);

    void send_close();
    void send_error();
    void receive_message();

    void read(unsigned char* buffer, size_t buffer_size, size_t offset, size_t length);
    void write(unsigned char* buffer, size_t buffer_size, size_t offset, size_t length);

    void write_Version_1_1(const TIDorb::core::comm::iiop::GIOPMessage& message,
                           const TIDorb::core::comm::iiop::RequestId request_id);

  public:
    /**
     * Creates a new connection in SERVER_MODE.
     */
    static Connection* server_connection(ConnectionManager* mngr, const TIDorb::core::comm::miop::ListenPoint& listen_point);

    /**
     * Creates a new connection in CLIENT_MODE.
     */
    static Connection* client_connection(ConnectionManager* mngr, const TIDorb::core::comm::miop::ListenPoint& listen_point);

};

} //comm
} //core
} //TIDorb

#endif
