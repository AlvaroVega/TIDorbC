/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/iiop/TargetAddress.h

 Revisions:

 Copyright 2000 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
 The copyright to the software program(s) is property of Telefonica I+D.
 The program(s) may be used and or copied only with the express written
 consent of Telefonica I+D or in acordance with the terms and conditions
 stipulated in the agreement/contract under which the program(s) have
 been supplied.
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
