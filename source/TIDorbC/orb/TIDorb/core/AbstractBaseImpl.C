/////////////////////////////////////////////////////////////////////////
//
// File:        AbstractBaseImpl.C
// Description:
//              
// Rel:         01.00
// Created:     November, 2002
//
// Revised:    
//
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
/////////////////////////////////////////////////////////////////////////

#include "TIDorb/core.h"


CORBA::AbstractBase_ptr CORBA::AbstractBase::_duplicate(CORBA::AbstractBase_ptr ab)
{
    //TODO jagd
    CORBA::Object_ptr reference = dynamic_cast<CORBA::Object_ptr>(ab);
    CORBA::ValueBase_ptr value  = dynamic_cast<CORBA::ValueBase_ptr>(ab);

    if(reference) {
        // Increment reference counter
        CORBA::Object::_duplicate(reference);
    } else if(value) {
    	CORBA::add_ref(value);
    }

    return ab;
}


CORBA::AbstractBase_ptr CORBA::AbstractBase::_narrow(CORBA::AbstractBase_ptr ab)
{
    return _duplicate(ab);
}


CORBA::AbstractBase_ptr CORBA::AbstractBase::_nil()
{
    return NULL;
}


CORBA::Object_ptr CORBA::AbstractBase::_to_object()
{
    CORBA::Object_ptr reference = dynamic_cast<CORBA::Object_ptr>(this);

    if(reference) {
        return CORBA::Object::_duplicate(reference);
    } else {
        return NULL;
    }
}




CORBA::ValueBase* CORBA::AbstractBase::_to_value()
{
    CORBA::ValueBase_ptr value = dynamic_cast<CORBA::ValueBase_ptr>(this);
    if (value) {
        CORBA::add_ref(value);
        return value;
    } else {
    	return NULL;
    }
}


CORBA::AbstractBase::AbstractBase()
{
}


CORBA::AbstractBase::AbstractBase(const CORBA::AbstractBase& val)
{
}


CORBA::Boolean CORBA::is_nil(CORBA::AbstractBase_ptr ab)
{
  return ab == NULL; 
}


void CORBA::release(CORBA::AbstractBase_ptr ab)
{
    CORBA::Object_ptr reference = dynamic_cast<CORBA::Object_ptr>(ab);
    CORBA::ValueBase_ptr value  = dynamic_cast<CORBA::ValueBase_ptr>(ab);

    if(reference) {
        CORBA::release(reference);
    } else if(value) {
    	CORBA::remove_ref(value);
    }
}
