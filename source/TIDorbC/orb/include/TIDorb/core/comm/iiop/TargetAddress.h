/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/iiop/TargetAddress.h

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

#ifndef _TIDORB_CORE_COMM_IIOP_TARGETADDRESS_H_
#define _TIDORB_CORE_COMM_IIOP_TARGETADDRESS_H_

namespace TIDorb {
namespace core {
namespace comm {
namespace iiop {

class TargetAddress {

private:
  void reset();
  
  
protected:
  CORBA::Short                                 _discriminator;
  TIDorb::core::comm::iiop::ObjectKey_ref      _object_key;
  TIDorb::core::comm::iiop::IORAddressingInfo* _ior;
  
  // MIOP extensions
  // TIDorb::core::comm::iiop::ProfileIIOP_ref _profile;
  TIDorb::core::iop::TaggedProfile_ref         _profile;
  // end MIOP extensions
  
public:
  TargetAddress();
  TargetAddress(const TargetAddress& other);
  ~TargetAddress();
  
  TIDorb::core::comm::iiop::ObjectKey* object_key() const;
  CORBA::Short object_key(TIDorb::core::comm::iiop::ObjectKey* value);

  // MIOP extensions
  // TIDorb::core::comm::iiop::ProfileIIOP* profile() const;
  // CORBA::Short profile(TIDorb::core::comm::iiop::ProfileIIOP* value);
  TIDorb::core::iop::TaggedProfile* profile() const;
  CORBA::Short profile(TIDorb::core::iop::TaggedProfile* value);
  // end MIOP extensions
  
  TIDorb::core::comm::iiop::IORAddressingInfo* ior() const;
  CORBA::Short ior(TIDorb::core::comm::iiop::IORAddressingInfo* value);
  
  CORBA::Short discriminator() const;

  TIDorb::core::comm::iiop::ObjectKey* getObjectKey() const;
  
  void read(TIDorb::core::cdr::CDRInputStream& in);
        
  
  static void write_object_key_address(
                            TIDorb::core::cdr::CDROutputStream& out,
                            const TIDorb::core::iop::IOR& ior);
  
  static void write_object_key_address(
                            TIDorb::core::cdr::CDROutputStream& out,
                            const TIDorb::core::comm::iiop::ObjectKey& key);

  static void write_profile_address(
                            TIDorb::core::cdr::CDROutputStream& out,
                            const TIDorb::core::iop::IOR& ior);

  static void write_ior_address(
                            TIDorb::core::cdr::CDROutputStream& out,
                            const TIDorb::core::iop::IOR& ior);
  
};
}// iiop
}// comm
}// core
}// TIDorb
#endif
