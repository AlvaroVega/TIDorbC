/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/iiop/GIOPHeader.h

 Revisions:

 Copyright 2000 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
 The copyright to the software program(s) is property of Telefonica I+D.
 The program(s) may be used and or copied only with the express written
 consent of Telefonica I+D or in acordance with the terms and conditions
 stipulated in the agreement/contract under which the program(s) have
 been supplied.
 ------------------------------------------------------------------------------ */

#include "TIDorb/core/comm/iiop.h"

#ifndef _TIDorb_core_comm_iiop_GIOPHEADER_H_
#define _TIDorb_core_comm_iiop_GIOPHEADER_H_

namespace TIDorb {
namespace core {
namespace comm {
namespace iiop {


class GIOPHeader {
 
  public:

  static CORBA::ULong HEADER_SIZE;

  Version        _version;
  CORBA::Boolean _byte_order;
  MsgType        _message_type;
  CORBA::ULong   _message_size;
  CORBA::Boolean _more_fragments;
  CORBA::Boolean _compressed;



  GIOPHeader();
  
  GIOPHeader(const GIOPHeader& other);
  
  GIOPHeader(const TIDorb::core::comm::iiop::Version& ver,
             TIDorb::core::comm::iiop::MsgType type);

  ~GIOPHeader();

  void fromByteArray(CORBA::Octet* array, CORBA::ULong size);

  void toByteArray(CORBA::Octet* header_buffer, CORBA::ULong size);

  const TIDorb::core::comm::iiop::Version& getVersion() const;

  TIDorb::core::comm::iiop::MsgType getMsgType() const;

  void setByteOrder(CORBA::Boolean order);

  CORBA::Boolean getByteOrder() const;

  void setMoreFragments(CORBA::Boolean more);

  CORBA::Boolean hasMoreFragments() const;

  void setSize(CORBA::ULong size);

  CORBA::ULong getSize() const;

  void setCompressed(CORBA::Boolean compressed);

  CORBA::Boolean getCompressed() const;

  void write(TIDorb::core::cdr::CDROutputStream& output) const;

  void read(TIDorb::core::cdr::CDRInputStream& input);

};

}// iiop
}// comm
}// core
}// TIDorb

ostream& operator<<(ostream&, 
                    const TIDorb::core::comm::iiop::GIOPHeader& header);

#endif
