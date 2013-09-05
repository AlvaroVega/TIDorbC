/////////////////////////////////////////////////////////////////////////
//
// File:        ContextCDR.C
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

#include "TIDorb/core/cdr.h"


namespace TIDorb {
namespace core {
namespace cdr {


ContextCDR::ContextCDR()
: m_father(0), 
  m_position_from_root(0),
  m_has_external_indirections(false)   
{
}

ContextCDR::ContextCDR(const ContextCDR* father, 
                       CDR::AbsolutePosition position_in_father)
: m_father(father),
  m_position_from_root(position_in_father),
  m_has_external_indirections(false)   
{
}


CDR::AbsolutePosition  ContextCDR::lookup(const CORBA::TypeCode_ptr tc) const
{
      
  CDR::AbsolutePosition position = m_positions_node.lookup(tc);

  if(position)
    return position;
 
  
  if (!m_father)
    return 0;  
  
  position = m_father->lookup(tc);
  
  if(position > 0) {
    ((ContextCDR *)this)->m_has_external_indirections = true;
  } 
  
  return position;
  
}

CDR::AbsolutePosition ContextCDR::lookup(const char* id) const
{
  return m_positions_node.lookup(id);
}

CDR::AbsolutePosition ContextCDR::lookup(const CORBA::ValueBase* val) const
{
  return m_positions_node.lookup(val);
}


const CORBA::TypeCode_ptr 
    ContextCDR::lookup_typecode (CDR::AbsolutePosition position) const
{
  if (position < m_position_from_root) {
    ((ContextCDR *)this)->m_has_external_indirections = true;
    
    if(m_father) {
      return m_father->lookup_typecode(position);
    } else {
      throw CORBA::MARSHAL("Invalid indirection");
    }
  } else { 
    return m_indirection_node.lookup_typecode(position);    
  }   
}

const char* ContextCDR::lookup_string (CDR::AbsolutePosition position) const
{
    return m_indirection_node.lookup_string(position);
}

const ValueTypeInfo& 
    ContextCDR::lookup_value (CDR::AbsolutePosition position) const
{
    return m_indirection_node.lookup_value(position);
}

void ContextCDR::put(CDR::AbsolutePosition position,
                     const CORBA::TypeCode_ptr tc)
{
  TIDThr::Synchronized synchro(mutex);

     if(position < m_position_from_root) {
         throw CORBA::INTERNAL("Indirection out of encapsulation");
     }
     m_indirection_node.put(position, tc);
     m_positions_node.put(tc, position);
}

/**
 * The string is at this position
 */
void ContextCDR::put(CDR::AbsolutePosition position, const char* str)
{
  TIDThr::Synchronized synchro(mutex);

     if (position < m_position_from_root) {
          throw CORBA::INTERNAL("Indirection out of encapsulation");
     }
     
     m_indirection_node.put(position, str);
     m_positions_node.put(str, position);
} 


void ContextCDR::put(CDR::AbsolutePosition position, 
                     const ValueTypeInfo& val_info)
{
  TIDThr::Synchronized synchro(mutex);

     if (position < m_position_from_root) {
          throw CORBA::INTERNAL("Indirection out of encapsulation");
     }

    m_indirection_node.put(position, val_info);
    //The value_type when demarshaled does not have to be the same    
   // m_positions_node.put(val_info.get_value_ref(), position);
}

void ContextCDR::put(CDR::AbsolutePosition position, 
                     const CORBA::ValueBase* value)
{
  TIDThr::Synchronized synchro(mutex);

    // warning: possible race condition when extracting a value from
    // several threads (from a any value)
    
     if (position < m_position_from_root) {
          throw CORBA::INTERNAL("Indirection out of encapsulation");
     }

     m_positions_node.put(value, position);
}

void ContextCDR::duplicate(ContextCDR* new_contextCDR)
{
  // Duplicate father if exists
  if (this->m_father != NULL){
    new_contextCDR->m_father = (ContextCDR*)this->m_father;
  }
  new_contextCDR->m_position_from_root = this->m_position_from_root;
  new_contextCDR->m_has_external_indirections = this->m_has_external_indirections;

  this->m_indirection_node.duplicate(&(new_contextCDR->m_indirection_node));
  this->m_positions_node.duplicate(&(new_contextCDR->m_positions_node));

}

} // cdr
} // core
} // TIDorb


