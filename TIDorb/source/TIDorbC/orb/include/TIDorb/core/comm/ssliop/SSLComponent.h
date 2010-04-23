//////////////////////////////////////////////////////////////////////////////////
//
// File:        SSLComponent.h
// Description: 
//
// Rel:         01.00
// Created:     June, 2009
// Author:      
//
// Revised:
//
// Copyright 2009 Telefonica, I+D. Printed in Spain (Europe). All Righ Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
///////////////////////////////////////////////////////////////////////////////////

#include "TIDorb/core/comm/ssliop.h"

#ifndef _TIDORB_CORE_COMM_SSLIOP_SSLCOMPONENT_H_
#define _TIDORB_CORE_COMM_SSLIOP_SSLCOMPONENT_H_

#include "SSLIOP.h"
#include "Security.h"


namespace TIDorb {
namespace core {
namespace comm {
namespace ssliop {

class SSLComponent : public virtual TIDorb::core::iop::TaggedComponent
{
private:
  char* _str;
  
  SSLIOP::SSL _ssl;

public:
  
  SSLComponent() throw (TIDThr::SystemException);
  
  SSLComponent(const SSLComponent& v) throw (TIDThr::SystemException);	
  
  SSLComponent(SSLIOP::SSL ssl) throw (TIDThr::SystemException);
  
  ~SSLComponent() throw (TIDThr::SystemException);


  //
  // Accesor methods
  //

  Security::AssociationOptions getTargetSupports();
  
  Security::AssociationOptions getTargetRequires();

  CORBA::UShort getSSLPort();

  const SSLIOP::SSL* getSSL();


  //
  // Marshaling methods
  //
  
  void write(TIDorb::core::cdr::CDROutputStream& out) const;
  
  void partial_read(TIDorb::core::cdr::CDRInputStream& input);
  
  const char* toString() const;
};
}// ssliop
}// comm
}// core
}// TIDorb
#endif
