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
