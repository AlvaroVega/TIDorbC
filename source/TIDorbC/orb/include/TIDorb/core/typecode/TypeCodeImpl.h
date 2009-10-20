/*//////////////////////////////////////////////////////////////////////////////////
//
// File:        TypeCodeImpl.h
// Description: Basic TypeCode implementation.
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
///////////////////////////////////////////////////////////////////////////////////*/

#include "TIDorb/core/typecode.h"

#ifndef _TIDorb_core_typecode_TypeCodeImpl_H_
#define _TIDorb_core_typecode_TypeCodeImpl_H_

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

class TypeCodeImpl : public CORBA::TypeCode,
                     public TIDThr::RefCounter
{

  public:
    TypeCodeImpl() throw (TIDThr::SystemException);
    TypeCodeImpl(CORBA::TCKind kind) throw (TIDThr::SystemException);
    virtual ~TypeCodeImpl() throw (TIDThr::SystemException);
      
    CORBA::TCKind kind() const;
    virtual CORBA::Boolean equal(CORBA::TypeCode_ptr tc) const;
    virtual CORBA::Boolean equivalent(CORBA::TypeCode_ptr tc) const;
    virtual CORBA::TypeCode_ptr get_compact_typecode() const;
    virtual const char* id() const;
    virtual const char* name() const;
    virtual CORBA::ULong member_count() const;
    virtual const char* member_name(CORBA::ULong index) const;
    virtual CORBA::TypeCode_ptr member_type(CORBA::ULong index) const;
    virtual CORBA::Any *member_label(CORBA::ULong index) const;
    virtual CORBA::TypeCode_ptr discriminator_type() const;
    virtual CORBA::Long default_index() const;
    virtual CORBA::ULong length() const;
    virtual CORBA::TypeCode_ptr content_type() const;
    virtual CORBA::UShort fixed_digits() const;
    virtual CORBA::Short fixed_scale() const;
    virtual CORBA::Visibility member_visibility(CORBA::ULong index) const;
    virtual CORBA::ValueModifier type_modifier() const;
    virtual CORBA::TypeCode_ptr concrete_base_type() const;

  //jagd optimizacion
    inline void _add_ref() throw (TIDThr::SystemException)
    {
      if (!isSimple())
       TIDThr::RefCounter::_add_ref();
    }

    inline void _remove_ref() throw (TIDThr::SystemException)
    {
      if (!isSimple())
       TIDThr::RefCounter::_remove_ref();
    }
    //Ejagd

    // TIDorb operations

    void set_exhaustive_equal(bool value);
    inline virtual bool isSimple()const {return true;}
 
	  
    /**
     * TypeCode marshalling
     */
    
    virtual void marshal(TIDorb::core::cdr::CDROutputStream& output);

    /**
     * TypeCode parameters demarshaling. In this case, theres nothing to do.
     * This operation will throw the <code>INTERNAL</code> exception.
     */
     
    virtual void partial_unmarshal(TIDorb::core::cdr::CDRInputStream& input);             
    
 /**
   * Skips the value asociated to the TypeCode. This operation is used by the   
   * TIDorb's Any implementation an the subclass <code>skip_value()</code>
   * operations.
   * @param input must be alwais a reference to a CDRInputStream object.
   */
       
    virtual bool skip_value(TIDorb::core::cdr::CDRInputStream& input) const;
    
    
   /**
    *  Copies and remarshals the given typecode value marshaled in an InputStream 
    *  to  a <code>es.tid.TIDorbj.core.CDRInputStream</code>.
    *  This method will alwais be invoked by this stream.
    *
    * @param input the <code>InputStream</code> where the value is marshaled
    * @param output the <code>es.tid.TIDorbj.core.CDRInputStream</code>
    * @pre the <code>TypeCode</code> must be a basic type
    */
    virtual void remarshal_value(TIDorb::core::cdr::CDRInputStream& input, 
                                 TIDorb::core::cdr::CDROutputStream& output) const;
    /**
     *  Compares two InputStream marshaled values of a given TypeCode  
     *  to  a <code>es.tid.TIDorbj.core.CDRInputStream</code>.
     *  This method will alwais be invoked by this stream.
     * @param input_a the <code>InputStream</code> where one value is marshaled
     * @param input_b the <code>InputStream</code> where the value value is marshaled
     * @pre <code>type</code>must be a basic type.
     */
    
    virtual bool values_equal (TIDorb::core::cdr::CDRInputStream& a_input, 
                               TIDorb::core::cdr::CDRInputStream& b_input) const;   
    /**
  	 * Dumps the description of a given string TypeCode.
  	 * @param output the output stream where the TypeCode will be dumped
  	 * @pre <code>type</code>must be a string type.
  	 */

    virtual void dump (ostream& output) const;

    /**
    * Dumps the description of a the marshaled value of a given TypeCode.
    * @param input the input stream where the value is marshaled
    * @param output the output stream where the value will be dumped
    * @pre the typecode must be a simple type
    */
    
    virtual bool dump_value(TIDorb::core::cdr::CDRInputStream& input,
	                    ostream& output) const;                                

  protected:

    CORBA::TCKind m_kind;	
    bool m_exhaustive_equal;

//MLG
  public:
    virtual void* _impl()
    {
        return this;
    }
    virtual const char* _typeid()
    {
        //return CORBA::string_dup("TypeCodeImpl");
        return "TypeCodeImpl";
    }
//EMLG
};

} // CORBA
}
}

#endif

