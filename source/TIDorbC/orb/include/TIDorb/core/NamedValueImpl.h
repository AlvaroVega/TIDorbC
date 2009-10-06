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
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
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
