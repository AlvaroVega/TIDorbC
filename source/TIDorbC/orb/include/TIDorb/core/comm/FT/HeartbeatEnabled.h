/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/FT/HeartbetEnabled.h
  
 Revisions:
 
 Copyright 2004 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
 The copyright to the software program(s) is property of Telefonica I+D.
 The program(s) may be used and or copied only with the express written
 consent of Telefonica I+D or in acordance with the terms and conditions
 stipulated in the agreement/contract under which the program(s) have
 been supplied.   
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
