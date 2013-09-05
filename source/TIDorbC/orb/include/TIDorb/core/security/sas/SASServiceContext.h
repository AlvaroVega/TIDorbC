/////////////////////////////////////////////////////////////////////////
//
// File:        SASServiceContext.h
// Description:
//              
// Rel:         01.00
// Created:     June, 2009
//
// Revised:
//
// Copyright 2009 Telefonica, I+D. Printed in Spain (Europe). All Rights
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
/////////////////////////////////////////////////////////////////////////

#include "TIDorb/core/security/sas.h"

#ifndef _TIDORB_CORE_SECURITY_SAS_SAS_SERVICE_CONTEXT_H_
#define _TIDORB_CORE_SECURITY_SAS_SAS_SERVICE_CONTEXT_H_

#include <CSI.h>


namespace TIDorb {
namespace core {
namespace security {
namespace sas {

/*
 * SASServiceContext at CSIv2 level 0
 */

class SASServiceContext : public TIDorb::core::comm::iiop::ServiceContext {

private:
  CSI::SASContextBody_ptr m_sas_context_body;

public:
  
  SASServiceContext() throw (TIDThr::SystemException);

  SASServiceContext(CSI::SASContextBody_ptr sas_context_body) 
    throw (TIDThr::SystemException);

  ~SASServiceContext() throw (TIDThr::SystemException);
  
  CSI::SASContextBody_ptr getSASContextBody();

  void partial_read(TIDorb::core::cdr::CDRInputStream& input);
  
  void write(TIDorb::core::cdr::CDROutputStream& output) const;

	
};
} //sas	
} //security
} //core
} //TIDorb
#endif

