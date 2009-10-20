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
