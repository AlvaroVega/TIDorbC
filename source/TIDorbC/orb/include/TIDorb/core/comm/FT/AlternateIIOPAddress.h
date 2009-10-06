/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/FT/AlternateIIOPAddress.h
  
 Revisions:
 
 Copyright 2004 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
 The copyright to the software program(s) is property of Telefonica I+D.
 The program(s) may be used and or copied only with the express written
 consent of Telefonica I+D or in acordance with the terms and conditions
 stipulated in the agreement/contract under which the program(s) have
 been supplied.   
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
