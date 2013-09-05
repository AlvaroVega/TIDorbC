/*/////////////////////////////////////////////////////////////////////////
//
// File:        WString_InternalMember.h
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

#ifndef _TIDORB_TYPES_WString_INTERNAL_MEMBER_H_
#define _TIDORB_TYPES_WString_INTERNAL_MEMBER_H_


#ifdef TIDORB_HAVE_IOSTREAM
#include <iostream>
#else
#include <iostream.h>
#endif

#ifdef TIDORB_HAVE_NAMESPACE_STD
using namespace std;
#endif


#include "CORBA/basic_types.h"
#include <wchar.h>

namespace TIDorb {
namespace types {

  // Tipo WString_InternalMember
  //
  // Este es un tipo auxiliar no accesible al usuario disenado para
  // tipar los miembros wstring de structs, unions y arrays.
  // Su implementacion es similar a la de CORBA::WString_var, pero se inicializa
  // por defecto con una cadena vacia
  //
  
  class WString_InternalMember 
  {
    public:
      WString_InternalMember(): m_ptr(CORBA::wstring_dup(L"")){}
    
      WString_InternalMember(CORBA::WChar *p): m_ptr(p){}
    
      WString_InternalMember(const CORBA::WChar *p) : m_ptr (CORBA::wstring_dup(p)){}
    
      WString_InternalMember(const CORBA::WString_var &s): m_ptr(CORBA::wstring_dup(s)) {};
      
      WString_InternalMember(const WString_InternalMember &s): m_ptr(CORBA::wstring_dup(s.m_ptr)) {};
    
      ~WString_InternalMember() {CORBA::wstring_free(m_ptr);}
    
      WString_InternalMember& operator=(CORBA::WChar *p)
      {
        if(m_ptr != p) {
          CORBA::wstring_free(m_ptr);
          m_ptr = p;
        }
        return *this;
      }
    
      WString_InternalMember& operator=(const CORBA::WChar *p)
      {
        if(m_ptr != p) {
          CORBA::wstring_free(m_ptr);
          m_ptr = CORBA::wstring_dup(p);
        }
        return *this;
      }
    
      WString_InternalMember& operator=(const CORBA::WString_var &s)
      {
        CORBA::wstring_free(m_ptr);
        m_ptr = CORBA::wstring_dup(s);
        return *this;
      }
    
     WString_InternalMember& operator=(const WString_InternalMember &s)
     {
      if(this != &s) {
        CORBA::wstring_free(m_ptr);
        m_ptr = CORBA::wstring_dup(s.m_ptr);
      }
      
      return *this;
    }
    
      operator CORBA::WChar*&() { return m_ptr;}
      operator const CORBA::WChar*&() const { 
         const CORBA::WChar*& aux = (const CORBA::WChar*&)  m_ptr;
         return aux;
      }

      const CORBA::WChar* in() const { return m_ptr;}
      CORBA::WChar*& inout() { return m_ptr;}
      CORBA::WChar*& out()
      {
        if(m_ptr)
          CORBA::wstring_free(m_ptr);
        return m_ptr;
      }
     
      CORBA::WChar* _retn()
      {
        CORBA::WChar* tmp_ptr = m_ptr;
        m_ptr = 0;
        return tmp_ptr;
      }
    
      CORBA::WChar& operator[](CORBA::ULong index)
      {
        return m_ptr[index];
      }
    
      CORBA::WChar operator[](CORBA::ULong index) const
      {
        return m_ptr[index];
      }
    
    private:
      CORBA::WChar* m_ptr;
  };

} // end of namespace types
} // end of namespace TIDorb

inline ostream& operator<<(ostream& os, const ::TIDorb::types::WString_InternalMember& s)
{
  os << (const ::CORBA::WChar*) s;
  return os;  
}

/*
istream& operator>>(istream& is, ::TIDorb::types::WString_InternalMember& s)
{
  is >> (::CORBA::WChar*) s;
  return is;  
}
*/

#endif

