/*/////////////////////////////////////////////////////////////////////////
//
// File:        String_InternalMember.h
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
/////////////////////////////////////////////////////////////////////////*/

#ifndef _TIDORB_TYPES_STRING_INTERNAL_MEMBER_H_
#define _TIDORB_TYPES_STRING_INTERNAL_MEMBER_H_

#include <string.h>

#ifdef TIDORB_HAVE_IOSTREAM
#include <iostream>
#else
#include <iostream.h>
#endif

#ifdef TIDORB_HAVE_NAMESPACE_STD
using namespace std;
#endif


namespace TIDorb {
namespace types {

  // Tipo String_InternalMember
  //
  // Este es un tipo auxiliar no accesible al usuario disenado para
  // tipar los miembros string de structs, unions y arrays.
  // Su implementacion es similar a la de CORBA::String_var, pero se inicializa
  // por defecto con una cadena vacia
  //
  
  class String_InternalMember 
  {
    public:
      String_InternalMember(): m_ptr(CORBA::string_dup("")){}
    
      String_InternalMember(char *p): m_ptr(p){}
    
      String_InternalMember(const char *p) : m_ptr (CORBA::string_dup(p)){}
    
      String_InternalMember(const CORBA::String_var &s): m_ptr(CORBA::string_dup(s)) {};
      
      String_InternalMember(const String_InternalMember &s): m_ptr(CORBA::string_dup(s.m_ptr)) {};
    
      ~String_InternalMember() {CORBA::string_free(m_ptr);}
    
      String_InternalMember& operator=(char *p)
      {
        if(m_ptr != p) {
          CORBA::string_free(m_ptr);
          m_ptr = p;
        }
        return *this;
      }
    
      String_InternalMember& operator=(const char *p)
      {
        if(m_ptr != p) {
          CORBA::string_free(m_ptr);
          m_ptr = CORBA::string_dup(p);
        }
        return *this;
      }
    
      String_InternalMember& operator=(const CORBA::String_var &s)
      {
        CORBA::string_free(m_ptr);
        m_ptr = CORBA::string_dup(s);
        return *this;
      }
    
    String_InternalMember& operator=(const String_InternalMember &s)
     {
      if(this != &s) {
        CORBA::string_free(m_ptr);
        m_ptr = CORBA::string_dup(s.m_ptr);
      }
      
      return *this;
    }
    
      operator char*&() { return m_ptr;}
      operator const char*&() const { 
         const char*& aux = (const char*&)  m_ptr;
         return aux;
      }

      const char* in() const { return m_ptr;}
      char*& inout() { return m_ptr;}
      char*& out()
      {
        if(m_ptr)
          CORBA::string_free(m_ptr);
        return m_ptr;
      }
     
      char* _retn()
      {
        char* tmp_ptr = m_ptr;
        m_ptr = 0;
        return tmp_ptr;
      }
    
      char& operator[](CORBA::ULong index)
      {
        return m_ptr[index];
      }
    
      char operator[](CORBA::ULong index) const
      {
        return m_ptr[index];
      }
    
    private:
      char* m_ptr;
  };

} // end of namespace types
} // end of namespace TIDorb

inline ostream& operator<<(ostream& os, const ::TIDorb::types::String_InternalMember& s)
{
  os << (const char *) s;
  return os;  
}

inline istream& operator>>(istream& is, ::TIDorb::types::String_InternalMember& s)
{
  is >> (char *) s;
  return is;  
}

#endif
