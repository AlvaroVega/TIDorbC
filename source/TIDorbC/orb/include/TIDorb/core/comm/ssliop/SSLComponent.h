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
