/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/iiop/GIOPHeader.h

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
