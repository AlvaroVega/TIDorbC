/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/iiop/UserExceptionEncoder.h

 Revisions:

 Copyright 2000 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
 The copyright to the software program(s) is property of Telefonica I+D.
 The program(s) may be used and or copied only with the express written
 consent of Telefonica I+D or in acordance with the terms and conditions
 stipulated in the agreement/contract under which the program(s) have
 been supplied.
 ------------------------------------------------------------------------------ */

#include "TIDorb/core/comm/iiop.h"

#ifndef _TIDORB_CORE_COMM_IIOP_USEREXCEPTIONENCODER_H_
#define _TIDORB_CORE_COMM_IIOP_USEREXCEPTIONENCODER_H_


namespace TIDorb {
namespace core {
namespace comm {
namespace iiop {

class UserSystemExceptionEncoder 
{
public:
  static void write(TIDorb::portable::OutputStream& out, 
                    CORBA::SystemException& sys);
  
  CORBA::UnknownUserException* read(TIDorb::portable::InputStream& in, 
                                    CORBA::ExceptionList_ptr types);
};
}// iiop
}// comm
}// core
}// TIDorb
#endif
