/*//////////////////////////////////////////////////////////////////////////////////
//
// File:        GenericValue.h
// Description: 
//
// Rel:         01.00
// Created:     
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
///////////////////////////////////////////////////////////////////////////////////*/

#include "TIDorb/core.h"
#include "TIDThr.h"

#ifndef _TIDORB_CORE_CDR_GENERICVALUE_H_
#define _TIDORB_CORE_CDR_GENERICVALUE_H_

#include "CORBA.h"

namespace TIDorb {
namespace core {
namespace util {

/**
 *  Class that allows the Value remarshalling with anys
 *  This value must be locally constrained to the buffer where it was
 *  readed. 
 */
     
class GenericValue: public virtual CORBA::DefaultValueRefCountBase 
{
    public:
        GenericValue(const CORBA::TypeCode_ptr type)
          :  m_type(type){}
        
        GenericValue(const CORBA::TypeCode_ptr type,
                     const TIDorb::core::cdr::PointerCDR& position)
          :  m_type(type), m_marshaled_value(position)
        {}
          
        GenericValue(const GenericValue& value);
        
        ~GenericValue() {}
                
        CORBA::ValueBase* _copy_value()
        {
            return new GenericValue(*this);
        }
        
        // TIDorb marshalling methods
        void _read(TIDorb::portable::InputStream& input);
        void _write(TIDorb::portable::OutputStream& out) const;
        
        CORBA::TypeCode_ptr _type() const
        {
            return CORBA::TypeCode::_duplicate(m_type.in());
        }        
        
        
    protected:
    
        CORBA::TypeCode_var m_type;
        TIDorb::core::cdr::PointerCDR m_marshaled_value; 
        // TIDThr::Mutex m_mutex;
};

} // end namespaces
}
}

#endif //_TIDORB_CORE_CDR_GENERICVALUE_H_
