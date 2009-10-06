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
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Right
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
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
