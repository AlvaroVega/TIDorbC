//////////////////////////////////////////////////////////////////////////////////
//
// File:        MIOPPacket.C
// Description: 
//
// Rel:         01.00
// Created:     September, 2002
// Author:      Juan A. Caceres Exposito      ( caceres@tid.es   )
//
// Revised:
//
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
///////////////////////////////////////////////////////////////////////////////////

#include "TIDorb/core/comm.h"




// Depending of unique_id filed of MIOPPacket, the MIOP header will  be
// between 24 to 252 
// This version of TIDorb uses a unique_id of 12 bits, as TAO uses.
CORBA::ULong TIDorb::core::comm::miop::MIOPPacket::HEADER_SIZE = 32; 




TIDorb::core::comm::miop::MIOPPacket::MIOPPacket
  (const TIDorb::core::comm::miop::Version& version,
   const TIDorb::core::comm::miop::UniqueId unique_id,
   TIDorb::core::cdr::BufferCDR* buffer,
   size_t num_packets)
   : _buffer_cdr(buffer), _output(NULL, buffer)
{
  CORBA::Octet  octet_version = (version.getMajor() << 4) + (version.getMinor());
  CORBA::Octet* id = (CORBA::Octet*) &unique_id;

  _flags = 0;
  if (_output.get_byte_order()) _flags |= 0x01;

  _number_of_packets = (CORBA::ULong) num_packets;

  // Write MIOP::PacketHeader_1_0

//   _output.write_char((CORBA::Char) 'M');
//   _output.write_char((CORBA::Char) 'I');
//   _output.write_char((CORBA::Char) 'O');
//   _output.write_char((CORBA::Char) 'P');                //magic

  CORBA::Long miop_word; 

  CORBA::Boolean _byte_order = TIDorb::core::cdr::CDR::LOCAL_BYTE_ORDER;
  if (_byte_order)
    miop_word=1347373389;
  else
    miop_word=1296650064;
  _output.write_long(miop_word);                        //magic

  _output.write_octet(octet_version);                   //hdr_version
  _output.skip(TIDorb::core::cdr::CDR::OCTET_SIZE  +    //flags;
               TIDorb::core::cdr::CDR::USHORT_SIZE +    //packet_length
               TIDorb::core::cdr::CDR::ULONG_SIZE);     //packet_number
  _output.write_ulong(_number_of_packets);              //number_of_packets
  //CORBA::ULong unique_id_len = TIDorb::core::cdr::CDR::ULONG_SIZE; /*RequestId*/
  CORBA::ULong unique_id_len = TIDorb::core::cdr::CDR::ULONGLONG_SIZE; /*RequestId*/

  // We Compose and use an UniqueId of 12 bytes (for TAO compatibility)
  //_output.write_ulong(unique_id_len+8);
  _output.write_ulong(unique_id_len+4);
  _output.write_octet_array(id, 0, unique_id_len);      //id
  // TODO: add a ORB timestamp 
  //for (int j = 0; j < 8; j++)
  for (int j = 0; j < 4; j++)
    _output.write_octet(0); // TODO: Change to _output.write_long(0);

}

  


size_t TIDorb::core::comm::miop::MIOPPacket::preparePacket
  (unsigned char* buffer, size_t buflen, size_t packet_number, size_t packet_length)
{
  // Set stop bit
  if (packet_number == _number_of_packets - 1) _flags |= 0x02;   	

  _output.rewind();
  _output.skip(TIDorb::core::cdr::CDR::LONG_SIZE +      //magic
               TIDorb::core::cdr::CDR::OCTET_SIZE);     //hdr_version
  _output.write_octet(_flags);                          //flags
  _output.write_ushort((CORBA::UShort) packet_length ); //packet_length
                     // + HEADER_SIZE );                //Workarround for TAO < 1.6.4 Interoperability
  _output.write_ulong((CORBA::ULong) packet_number);    //packet_number
  _output.skip(TIDorb::core::cdr::CDR::ULONG_SIZE +     //number_of_packets
               //(2 * TIDorb::core::cdr::CDR::ULONG_SIZE)); //id (sequence<octet>)
               (4 * TIDorb::core::cdr::CDR::ULONG_SIZE)); //id (sequence<octet>)
  _output.write_octet_array(buffer, 0, packet_length);    // giop request

  return (HEADER_SIZE + packet_length);
}




unsigned char* TIDorb::core::comm::miop::MIOPPacket::getPacketBuffer()
{
  return _buffer_cdr->get_chunk(0)->get_buffer();
}
