/////////////////////////////////////////////////////////////////////////
//
// File:        AbstractLocalRequest.h
// Description:
//              
// Rel:         01.00
// Created:     November, 2002
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
/////////////////////////////////////////////////////////////////////////

#include "TIDorb/core/poa.h"

#ifndef _TIDORB_CORE_POA_AbstractLocalRequest_H_
#define _TIDORB_CORE_POA_AbstractLocalRequest_H_


namespace TIDorb {
namespace core {
namespace poa {


class AbstractLocalRequest : public TIDorb::core::poa::QueuedRequest,
                             public TIDThr::RefCounter /*PRA*/
{
public:
  AbstractLocalRequest(TIDorb::core::poa::POAKey* poakey,
                       TIDorb::core::poa::POAImpl* root_poa);
  
  ~AbstractLocalRequest() throw(TIDThr::SystemException);
  void destroy();
  
//   void* _impl();
//   const char* _typeid();
  
};


} //poa
} //core
} //TIDorb

#endif
