/////////////////////////////////////////////////////////////////////////
//
// File:        GenericValue.C
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

#include "TIDorb/core/util/GenericValue.h"

using TIDorb::core::typecode::ValueTypeCode;
using TIDorb::core::cdr::CDRInputStream;
using TIDorb::core::cdr::CDROutputStream;

namespace TIDorb {
namespace core {
namespace util {
 

GenericValue::GenericValue(const GenericValue& value)
    :  m_type(value.m_type), m_marshaled_value(value.m_marshaled_value)
{
}
       
       
   
void GenericValue::_read(TIDorb::portable::InputStream& input)
{    
    //jagd
    //CDRInputStream& cdr_input = dynamic_cast< CDRInputStream& > (input);    
    CDRInputStream& cdr_input =  * ( CDRInputStream *)  (&input);    
    m_marshaled_value = cdr_input.get_pointer();
     
    //jagd       
    //ValueTypeCode* type = dynamic_cast<ValueTypeCode*> (m_type.in());
    ValueTypeCode* type = (ValueTypeCode*) (m_type.in());
        
    type->partial_skip_value(cdr_input);
}

void GenericValue::_write(TIDorb::portable::OutputStream& out) const
{     
    if(m_marshaled_value.get_buffer()) {
        
        ValueTypeCode* type =
            //jagd
            //dynamic_cast<ValueTypeCode*> (m_type.in());
            (ValueTypeCode*) (m_type.in());
        
        //CDROutputStream& cdr_output = dynamic_cast <CDROutputStream& > (out);
        CDROutputStream& cdr_output = *(CDROutputStream* )  (&out);
        
        CDRInputStream cdr_input (cdr_output.tidorb(), 
                                  m_marshaled_value.get_buffer());
        
        cdr_input.goBack(m_marshaled_value);
                  
        type->partial_remarshal_value(cdr_input, cdr_output);
    } else {
        throw CORBA::INTERNAL("Invalid Generic Value");
    }    
}
 
        
} // namespace
}
}
