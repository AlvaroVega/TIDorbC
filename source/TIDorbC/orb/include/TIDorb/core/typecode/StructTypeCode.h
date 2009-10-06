/*//////////////////////////////////////////////////////////////////////////////////
//
// File:        StructTypeCode.h
// Description: TypeCode implementation for structs.
//
// Rel:         01.00
// Created:     September, 2002
// Author:      Juan A. Caceres Exposito      ( caceres@tid.es   )
//
// Revised:
//
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Righ Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
///////////////////////////////////////////////////////////////////////////////////*/

#include "TIDorb/core/typecode.h"

#ifndef _TIDorb_core_typecode_StructTypeCode_H_
#define _TIDorb_core_typecode_StructTypeCode_H_

#include "CORBA.h"
#include "TIDThr.h"


#ifdef TIDORB_HAVE_IOSTREAM
  #include <iostream>
#else
  #include <iostream.h>
#endif

#ifdef TIDORB_HAVE_NAMESPACE_STD
  using namespace std;
#endif


namespace TIDorb {
namespace core {
namespace typecode {


class StructTypeCode : public ComplexTypeCode
{
  public:
    StructTypeCode()
       throw (TIDThr::SystemException);

    StructTypeCode(const char* id, const char* name, CORBA::StructMemberSeq* members)
       throw (TIDThr::SystemException);

    ~StructTypeCode()
       throw (TIDThr::SystemException);

    CORBA::Boolean equal(CORBA::TypeCode_ptr tc) const;
    CORBA::Boolean equivalent(CORBA::TypeCode_ptr) const;
    CORBA::ULong member_count() const;
    const char* member_name(CORBA::ULong index) const;
    CORBA::TypeCode_ptr member_type(CORBA::ULong index) const;

    /**
     * TypeCode marshalling
     */

    virtual bool skip_value(TIDorb::core::cdr::CDRInputStream& input) const
		{
		  CORBA::ULong length = m_members->length();
		  //TypeCodeImpl* tc = 0;
                  //jagd 5
                  CORBA::StructMember * buffer= m_members->get_buffer(); 
                  for(CORBA::ULong i = 0; i < length; i++)
                  {
                    if(! ((TypeCodeImpl*)((CORBA::TypeCode *)(buffer->type)))->skip_value(input))
                      return false;
                    buffer++;
                  }
                  return true; 

                /*
		  for(CORBA::ULong i = 0; i < length; i++) {
		    //tc = dynamic_cast<TypeCodeImpl*>((CORBA::TypeCode_ptr)((*m_members)[i]).type);
		    //jagd 5 tc = (TypeCodeImpl*)((CORBA::TypeCode_ptr)(*m_members)[i].type)->_impl();
		    tc = (TypeCodeImpl*)((CORBA::TypeCode_ptr)(*m_members)[i].type);
		    if(!tc->skip_value(input))
		      return false;
		  }
		
		  return true;
                */
		}    	

    virtual void remarshal_value(TIDorb::core::cdr::CDRInputStream& input,
                                 TIDorb::core::cdr::CDROutputStream& output) const;

    virtual bool values_equal (TIDorb::core::cdr::CDRInputStream& a_input,
                               TIDorb::core::cdr::CDRInputStream& b_input) const;

    virtual void dump (ostream& output) const;

    virtual bool dump_value(TIDorb::core::cdr::CDRInputStream& input, ostream& output) const;

    void write_params(TIDorb::core::cdr::CDROutputStream& output);
    void read_params(TIDorb::core::cdr::CDRInputStream& input);

  protected:
    CORBA::StructMemberSeq* m_members;

};


}
}
}

#endif
