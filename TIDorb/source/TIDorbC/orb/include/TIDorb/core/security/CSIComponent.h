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
