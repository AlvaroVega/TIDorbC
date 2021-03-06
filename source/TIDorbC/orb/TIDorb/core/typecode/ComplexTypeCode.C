/////////////////////////////////////////////////////////////////////////
//
// File:        ComplexTypeCode.C
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

#include "TIDorb/core/typecode.h"
#include <string.h>




TIDorb::core::typecode::ComplexTypeCode::ComplexTypeCode(CORBA::TCKind kind)
  throw (TIDThr::SystemException)
  : TypeCodeImpl(kind), m_name(0), m_id(0), m_marshaled_params(0)
{
}




TIDorb::core::typecode::ComplexTypeCode::ComplexTypeCode(CORBA::TCKind kind,
                                                         const char* id,
                                                         const char* name)
  throw (TIDThr::SystemException)
  : TypeCodeImpl(kind), m_marshaled_params(0)
{
  m_name = CORBA::string_dup(name);
  m_id = CORBA::string_dup(id);
}




TIDorb::core::typecode::ComplexTypeCode::~ComplexTypeCode()
  throw (TIDThr::SystemException)
{
  CORBA::string_free(m_name);
  CORBA::string_free(m_id);
}




CORBA::Boolean TIDorb::core::typecode::ComplexTypeCode::equal(CORBA::TypeCode_ptr tc) const
{
  if (!TypeCodeImpl::equal(tc))
    return false;

  try {
    return (!strcmp(m_id , tc->id()) && !strcmp(m_name, tc->name()));
  } catch (const CORBA::TypeCode::BadKind& bk) {
    return false;
  }
}




CORBA::Boolean TIDorb::core::typecode::ComplexTypeCode::equivalent(CORBA::TypeCode_ptr tc) const
{
  if (!TypeCodeImpl::equivalent(tc))
    return false;

  try {
    return (!strcmp(m_id , tc->id()) && !strcmp(m_name, tc->name()));
  } catch (const CORBA::TypeCode::BadKind& bk) {
    return false;
  }
}




const char* TIDorb::core::typecode::ComplexTypeCode::id() const
{
  return m_id;
}




const char* TIDorb::core::typecode::ComplexTypeCode::name() const
{
  return m_name;
}




bool TIDorb::core::typecode::ComplexTypeCode::isSimple()const
{
  return false;
}




void TIDorb::core::typecode::ComplexTypeCode::marshal
       (TIDorb::core::cdr::CDROutputStream& output)
{
  TIDThr::Synchronized synchro(m_recursive_mutex);

  TIDorb::core::TIDORB* output_orb = NULL;
  output_orb = (TIDorb::core::TIDORB*)(output.orb());
  
  if (output_orb)
  {
  	TIDorb::core::typecode::TypeCodeCache* cache = NULL;
  	
  	cache = output_orb->getTypeCodeCache();
  	
  	if (cache)
  		cache->put(CORBA::TypeCode::_duplicate(this));
  }

  // cheks if the typecode has been marshaled before


  TIDorb::core::cdr::CDR::AbsolutePosition previous_position = 0;

   // "Indirections are not "freestanding", but only exist inside some other encoded TypeCode

  //  if(!output.getContextCDR()->is_root_context()) {
  if(!output.get_context()->is_root_context()) {

    previous_position = output.get_context()->lookup(this);
  }

  if (previous_position) { // marshal indirection
    output.write_indirection(previous_position);
    return;
  }

  // aligns to get the typeCode position

  output.alignment(TIDorb::core::cdr::CDR::LONG_SIZE);

  // calculate the typecode position

  TIDorb::core::cdr::CDR::AbsolutePosition typecode_position;

   typecode_position = output.get_absolute_position();

  // saves the typecode position

   output.get_context()->put(typecode_position,this);

  // write the tc_kind value

   output.write_long(m_kind);

  if(m_marshaled_params.is_null()) {
    TIDorb::core::cdr::CDROutputStream* encapsulation = output.create_encapsulation();
    write_params(*encapsulation);
    output.write_buffer(*(encapsulation->get_buffer()));

    if(!encapsulation->get_context()->has_external_indirections())
      m_marshaled_params = encapsulation->get_buffer();

    delete encapsulation;
   } else {
    output.write_buffer(*m_marshaled_params);
  }
}




void TIDorb::core::typecode::ComplexTypeCode::partial_unmarshal
       (TIDorb::core::cdr::CDRInputStream& input)
{
  TIDorb::core::cdr::CDRInputStream* encapsulation = input.read_encapsulation();

  read_params(*encapsulation);

  delete encapsulation;
}




void TIDorb::core::typecode::ComplexTypeCode::write_params
  (TIDorb::core::cdr::CDROutputStream& output)
{
  output.write_string(m_id);
  output.write_string(m_name);
}




void TIDorb::core::typecode::ComplexTypeCode::read_params
  (TIDorb::core::cdr::CDRInputStream& input)
{
  if (m_id){
      CORBA::string_free(m_id);
  }

  input.read_string(m_id);

  if (m_name){
      CORBA::string_free(m_name);
  }

  input.read_string(m_name);
}




void TIDorb::core::typecode::ComplexTypeCode::dump_params (ostream& output) const
{
  output << "repositoryId= " << m_id << ", name=" << m_name;
}
