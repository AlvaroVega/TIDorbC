//////////////////////////////////////////////////////////////////////////////////
//
// File:        NamedValueImpl.h
// Description: CORBA::NamedValue implementation.
//
// Rel:         01.00
// Created:     September, 2002
// Author:      Juan A. Caceres Exposito      ( caceres@tid.es   )
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

#include "TIDorb/core.h"

#ifndef __TIDorb_core_NamedValueImpl_H_
#define __TIDorb_core_NamedValueImpl_H_

#include "CORBA.h"
#include "TIDThr.h"

namespace TIDorb {
namespace core {

class NamedValueImpl : public CORBA::NamedValue
                       //jagd 3 public TIDThr::RefCounter
{
  public:
    NamedValueImpl(){};

    NamedValueImpl(char* name, CORBA::Any* value, 
                   CORBA::Flags flags)  throw (TIDThr::SystemException);

    NamedValueImpl(char* name, CORBA::Boolean consume_name, CORBA::Any* value, 
                   CORBA::Flags flags)  throw (TIDThr::SystemException);
    
    ~NamedValueImpl()  throw (TIDThr::SystemException);
    
    const char *name() const;
    CORBA::Any *value() const;    
    CORBA::Flags flags() const;
    
    void release_out(bool value);
    

    
//  protected:
//  jagd 4 optimizacion
    bool m_release_out;  

    CORBA::Boolean m_consume_name;  
    
    char * m_name;
    //const char * m_name;
    CORBA::Any* m_value;
    CORBA::Flags m_flags;

//MLG
  public:
    void* _impl();
    const char* _typeid();
//EMLG
};

}
}


#endif
