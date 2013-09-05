/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/FT/AlternateIIOPAddress.h
  
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

#include "TIDorb/core.h"


#ifndef _TIDORB_CORE_COMM_FT_ALTERNATE_IIOP_ADDRESS_H_
#define _TIDORB_CORE_COMM_FT_ALTERNATE_IIOP_ADDRESS_H_


namespace TIDorb {
namespace core {
namespace comm {
namespace FT {

class AlternateIIOPAddress : public virtual TIDorb::core::iop::TaggedComponent
{
private:
  char* _str;

public:
  TIDorb::core::comm::iiop::ListenPoint _listen_point;

  AlternateIIOPAddress() throw (TIDThr::SystemException);
  AlternateIIOPAddress(const TIDorb::core::comm::iiop::ListenPoint& addr)
    throw (TIDThr::SystemException);

  AlternateIIOPAddress(const AlternateIIOPAddress& other)
    throw (TIDThr::SystemException);

  ~AlternateIIOPAddress() throw (TIDThr::SystemException);

  bool operator== (const AlternateIIOPAddress& other) const;

  void write(TIDorb::core::cdr::CDROutputStream& out) const;

  void partial_read(TIDorb::core::cdr::CDRInputStream& input);

  const char* toString() const;
};

}// FT
}// comm
}// core
}// TIDorb

#endif
