//////////////////////////////////////////////////////////////////////////////////
//
// File:        CSIComponent.h
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

#include "TIDorb/core/security.h"

#ifndef _TIDORB_CORE_SECURITY_CSI_COMPONENT_H_
#define _TIDORB_CORE_SECURITY_CSI_COMPONENT_H_

#include "CSIIOP.h"




namespace TIDorb {
namespace core {
namespace security {

class CSIComponent : public virtual TIDorb::core::iop::TaggedComponent
{
private:
  char* _str;

  CSIIOP::CompoundSecMechList _mec_list;

public:
  
  CSIComponent() throw (TIDThr::SystemException);
  
  CSIComponent(const CSIComponent& v) throw (TIDThr::SystemException);	
  
  CSIComponent(CSIIOP::CompoundSecMechList mec_list) throw (TIDThr::SystemException);
  
  ~CSIComponent() throw (TIDThr::SystemException);

  //
  // TODO: Add some methods to get information in CSIComponent
  // 
  const CSIIOP::CompoundSecMechList* getCompoundSecMechList(); 
  

  //
  // Marshaling methods
  //
  
  void write(TIDorb::core::cdr::CDROutputStream& out) const;
  
  void partial_read(TIDorb::core::cdr::CDRInputStream& input);
  
  const char* toString() const;
};

}// security
}// core
}// TIDorb
#endif
