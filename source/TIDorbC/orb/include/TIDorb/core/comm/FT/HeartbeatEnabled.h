/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/FT/HeartbetEnabled.h
  
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


#ifndef _TIDORB_CORE_COMM_FT_HEARTBEAT_ENABLED_H_
#define _TIDORB_CORE_COMM_FT_HEARTBEAT_ENABLED_H_


namespace TIDorb {
namespace core {
namespace comm {
namespace FT {

class HeartbeatEnabled : public TIDorb::core::iop::TaggedComponent
{
private:
  char* _str;

public:
  CORBA::Boolean heartbeat_enabled;

  HeartbeatEnabled() throw (TIDThr::SystemException);
  HeartbeatEnabled(CORBA::Boolean enabled) throw (TIDThr::SystemException);

  HeartbeatEnabled(const HeartbeatEnabled& other)
    throw (TIDThr::SystemException);

  ~HeartbeatEnabled() throw (TIDThr::SystemException);

  bool operator== (const HeartbeatEnabled& other) const;

  void write(TIDorb::core::cdr::CDROutputStream& out) const;

  void partial_read(TIDorb::core::cdr::CDRInputStream& input);

  const char* toString() const;
};

}// FT
}// comm
}// core
}// TIDorb

#endif
