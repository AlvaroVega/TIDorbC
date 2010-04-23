#ifndef _CORBA_WString_h_
#define _CORBA_WString_h_ 


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
#include <stdlib.h>

namespace CORBA {
  
inline WChar *wstring_alloc(ULong len)
{
  return (WChar*) malloc(sizeof(WChar)*(len+1));
}

inline WChar *wstring_dup(const WChar* str)
{
  if(str == 0) {
    return 0;
  } 
  else{
    
    WChar * tmp_wstr = 0;
    size_t str_length = wcslen(str);
    
    tmp_wstr = CORBA::wstring_alloc(str_length);
    
    if (str_length)
#if (!(defined(_XOPEN_SOURCE) && (_XOPEN_VERSION - 0 == 4)))
      wmemcpy(tmp_wstr, str, str_length);
#else
      memcpy (tmp_wstr, str, str_length * sizeof (wchar_t));
#endif

    tmp_wstr[str_length] = L'\0';
  
    return tmp_wstr;
  }
}

inline void wstring_free(WChar* str)
{
  if (str != 0) {
    free(str);
    str = 0;
  }
}  

class WString_var
{
  friend class WString_out;
  
  public:
    WString_var(): m_ptr(0){}
    
    WString_var(WChar* p): m_ptr(p){}
    
    WString_var(const WChar* p) : m_ptr (wstring_dup(p)){}
    
    WString_var(const WString_var &s): m_ptr(wstring_dup(s.m_ptr)) {};
    
    ~WString_var() {wstring_free(m_ptr);}
    
    WString_var& operator=(WChar* p)
    {
      if(m_ptr != p) {
        wstring_free(m_ptr);
        m_ptr = p;
      }
      
      return *this;
    }
    
    WString_var& operator=(const WChar* p)
    {
      if(m_ptr != p) {
        wstring_free(m_ptr);
        m_ptr = wstring_dup(p);
      }
      
      return *this;
    }
    
    WString_var& operator=(const WString_var &s)
     {
      if(this != &s) {
        wstring_free(m_ptr);
        m_ptr = wstring_dup(s.m_ptr);
      }
      
      return *this;
    }
    
    operator WChar*&() { return m_ptr;}
    operator const WChar*&() const 
    {
      const WChar*& aux = (const WChar*&) m_ptr;
      return aux;
    }
    const WChar* in() const { return m_ptr;}
    WChar*& inout() { return m_ptr;}
    WChar*& out()
    {
      if(m_ptr)
        wstring_free(m_ptr);
      return m_ptr;
    }
     
    WChar* _retn()
    {
      WChar* tmp_ptr = m_ptr;
      m_ptr = 0;
      return tmp_ptr;
    }
    
    WChar& operator[](ULong index)
    {
      return m_ptr[index];
    }
    
    WChar operator[](ULong index) const
    {
      return m_ptr[index];
    }
    
  private:
    WChar* m_ptr;
};

class WString_out
{
  public:
    WString_out(WChar*& p) : m_ptr(p) 
    { 
      m_ptr = 0;
    }
    
    WString_out(WString_var& p) : m_ptr(p.m_ptr)
    {
      wstring_free(m_ptr); 
      m_ptr = 0;
    }
    
    WString_out(const WString_out& s) : m_ptr(s.m_ptr) {}
    
    WString_out& operator=(const WString_out& s) 
    {
      m_ptr = s.m_ptr; 
      return *this;
    }
    
    WString_out& operator=(WChar* p) 
    {
      m_ptr = p; 
      return *this;
    }
    
    WString_out& operator=(const WChar* p) 
    {
      m_ptr = wstring_dup(p); 
      return *this;
    }
    
    operator WChar*&() { return m_ptr; }
    operator const WChar*() const { return m_ptr;}
    
    WChar*& ptr() { return m_ptr; }
    
    private:
      WChar*& m_ptr;
      
      // assignment from WString_var disallowed
      void operator=(const WString_var&){}
};

} // CORBA


inline ostream& operator<<(ostream& os, const ::CORBA::WString_var& s)
{
  os << (const ::CORBA::WChar*) s;
  return os;  
}

inline ostream& operator<<(ostream& os, const ::CORBA::WString_out& s)
{
  os << (const ::CORBA::WChar*) s;
  return os;  
}

/*
istream& operator>>(istream& is, ::CORBA::WString_var& s)
{
  is >> (::CORBA::WChar*) s;
  return is;  
}

istream& operator>>(istream& is, ::CORBA::WString_out& s)
{
  is >> (::CORBA::WChar*) s;
  return is;  
}
*/


#endif
