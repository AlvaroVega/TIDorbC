//////////////////////////////////////////////////////////////////////////////////
//
// File:        GIOPHeader.C
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

#include "TIDorb/core/comm/iiop/GIOPHeader.h"

namespace TIDorb {
namespace core {
namespace comm {
namespace iiop {
    


CORBA::ULong TIDorb::core::comm::iiop::GIOPHeader::HEADER_SIZE = 12;


GIOPHeader::GIOPHeader()
: _version (TIDorb::core::comm::iiop::Version::VERSION_1_2),
  _byte_order (TIDorb::core::cdr::CDR::LOCAL_BYTE_ORDER),
  _message_type (TIDorb::core::comm::iiop::Request),
  _message_size (0),
  _more_fragments (false),
  _compressed (false)
{
}

GIOPHeader::GIOPHeader(const GIOPHeader& other)
: _version (other._version),
  _byte_order (other._byte_order),
  _message_type (other._message_type),
  _message_size (other._message_size),
  _more_fragments (other._more_fragments),
  _compressed (other._compressed)
{
}

TIDorb::core::comm::iiop::GIOPHeader::GIOPHeader(const TIDorb::core::comm::iiop::Version& ver,
                                                 TIDorb::core::comm::iiop::MsgType type)
: _version (ver),
  _byte_order (TIDorb::core::cdr::CDR::LOCAL_BYTE_ORDER),
  _message_type (type),
  _message_size (0),
  _more_fragments (false),
  _compressed (false)
{
}




TIDorb::core::comm::iiop::GIOPHeader::~GIOPHeader() {}





const TIDorb::core::comm::iiop::Version& TIDorb::core::comm::iiop::GIOPHeader::getVersion() const
{
  return _version;
}




TIDorb::core::comm::iiop::MsgType TIDorb::core::comm::iiop::GIOPHeader::getMsgType() const
{
  return _message_type;
}




void TIDorb::core::comm::iiop::GIOPHeader::setByteOrder(CORBA::Boolean order)
{
  _byte_order = order;
}




CORBA::Boolean TIDorb::core::comm::iiop::GIOPHeader::getByteOrder() const
{
  return _byte_order;
}




void TIDorb::core::comm::iiop::GIOPHeader::setMoreFragments(CORBA::Boolean more)
{
  _more_fragments = more;
}




bool TIDorb::core::comm::iiop::GIOPHeader::hasMoreFragments() const
{
  if (_version._minor == 0)
    return false;
  else
    return _more_fragments;
}



void TIDorb::core::comm::iiop::GIOPHeader::setSize(CORBA::ULong size)
{
  _message_size = size;
}




CORBA::ULong TIDorb::core::comm::iiop::GIOPHeader::getSize() const
{
  return _message_size;
}





void TIDorb::core::comm::iiop::GIOPHeader::setCompressed(CORBA::Boolean compressed)
{
  _compressed = compressed;
}

CORBA::Boolean TIDorb::core::comm::iiop::GIOPHeader::getCompressed() const
{
  return _compressed;
}


void TIDorb::core::comm::iiop::GIOPHeader::write(TIDorb::core::cdr::CDROutputStream& output) const
{
  //jagd cambia 4 octetos por un long
   
  //output.write_octet((CORBA::Octet)'G');
  //output.write_octet((CORBA::Octet)'I');
  //output.write_octet((CORBA::Octet)'O');
  //output.write_octet((CORBA::Octet)'P');
  
  //este numero es GIOP pasado a long
  if (_byte_order) {
    if (_compressed)
      output.write_long(1347373402);
    else
      output.write_long(1347373383); 
  } else {
    if (_compressed)
      output.write_long(1514753872);
    else
      output.write_long(1195986768);
  }

  _version.write(output);

  if (_version == Version::VERSION_1_0)
    output.write_boolean(_byte_order);
  else {
    CORBA::ULong flag = 0;
    if (_byte_order)
      flag |= 0x1;
    if (_more_fragments)
      flag |= 0x2;
    output.write_octet((CORBA::Octet)flag);
  }

  output.write_octet((CORBA::Octet)_message_type);
  output.write_ulong(_message_size);
}

void TIDorb::core::comm::iiop::GIOPHeader::read(TIDorb::core::cdr::CDRInputStream& input)
{
  /* jagd
  CORBA::Octet a1,a2,a3,a4;
  input.read_octet(a1);
  input.read_octet(a2);
  input.read_octet(a3);
  input.read_octet(a4);
  if ((a1 != (CORBA::Octet)'G') ||
      (a2 != (CORBA::Octet)'I') ||
      (a3 != (CORBA::Octet)'O') ||
      (a4 != (CORBA::Octet)'P'))
    throw CORBA::MARSHAL("Bad GIOP Message header: Invalid header identifier.",
                         0, CORBA::COMPLETED_NO);
  */

  CORBA::Long giop;
  input.read_long(giop);
  if ( ( _byte_order && ( (giop!=1347373383) && (giop!=1347373402)) ) || 
       (!_byte_order && ( (giop!=1195986768) && (giop!=1514753872)) ) )
    throw CORBA::MARSHAL("Bad GIOP Message header: Invalid header identifier.",
                         0, CORBA::COMPLETED_NO);

  _compressed = ( (giop==1347373402) || (giop==1514753872) );

  _version.read(input);

  //unsigned char* flag = input.read_octet();
  CORBA::Octet flag;
  input.read_octet(flag);

  if (_version == Version::VERSION_1_0)
    _byte_order = (flag != 0);
  else {
    _byte_order = ((flag & 0x1) != 0);
    _more_fragments = ((flag & 0x2) != 0);
  }

  input.set_byte_order(_byte_order);

  CORBA::Octet tipo;
  input.read_octet(tipo);
  _message_type = TIDorb::core::comm::iiop::MsgType_from_int((CORBA::ULong)tipo); //MCPG

  //_message_size = input.read_ulong();
  input.read_ulong(_message_size);
}

} 
}
}
} // namespace TIDorb::core::comm::iiop


ostream& operator<<(ostream& os, 
                    const TIDorb::core::comm::iiop::GIOPHeader& header)
{
      os << TIDorb::core::comm::iiop::toString(header.getMsgType())
         << " { "
       << header.getVersion().toString() << " "
       << ((header.hasMoreFragments()) ? "[FRAGMENTED] " : "")
       << ((header.getByteOrder()) ? "[LITTLE_ENDIAN] " : "[BIG_ENDIAN] ")
       << "[SIZE: " << header.getSize() << "]"
       << " }"
       << ends;

  return os;
}
