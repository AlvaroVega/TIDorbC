#ifndef _CORBA_String_h_
#define _CORBA_String_h_


#ifdef TIDORB_HAVE_IOSTREAM
#include <iostream>
#else
#include <iostream.h>
#endif

#ifdef TIDORB_HAVE_NAMESPACE_STD
  using namespace std;
#endif

#include <stdlib.h>
#include <string.h>


namespace CORBA {
  
inline char *string_alloc(ULong len)
{
  //  return new char[len+1];
  return (char*) malloc(len+1); // asumimos sizeof(char) = 1 !!
}

inline char *string_dup(const char* str)
{

  if (str == NULL){
    return NULL;
  }
  else{
    //     char *aux = NULL;
    //     CORBA::ULong length = strlen(str);
    
    //     aux = CORBA::string_alloc(length);
    //     if (length)
    //       memcpy(aux, str, length);
    //     aux[length] = '\0';
    
    //     return aux;
    return strdup(str);
  }

}

inline void string_free(char * str)
{
  if (str != NULL){
    //    delete[] str;
    free(str);
    str = NULL;
  }
}  

class String_var
{
  friend class String_out;
  
  public:
    String_var(): m_ptr(0){}
    
    String_var(char *p): m_ptr(p){}
    
    String_var(const char *p) : m_ptr (string_dup(p)){}
    
    String_var(const String_var &s): m_ptr(string_dup(s.m_ptr)) {};
    
    ~String_var() {string_free(m_ptr);}
    
    String_var& operator=(char *p)
    {
      if(m_ptr != p) {
        string_free(m_ptr);
        m_ptr = p;
      }
      
      return *this;
    }
    
    String_var& operator=(const char *p)
    {
      if(m_ptr != p) {
        string_free(m_ptr);
        m_ptr = string_dup(p);
      }
      
      return *this;
    }
    
    String_var& operator=(const String_var &s)
     {
      if(this != &s) {
        string_free(m_ptr);
        m_ptr = string_dup(s.m_ptr);
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
        string_free(m_ptr);
      return m_ptr;
    }
     
    char* _retn()
    {
      char* tmp_ptr = m_ptr;
      m_ptr = 0;
      return tmp_ptr;
    }
    
    char& operator[](ULong index)
    {
      return m_ptr[index];
    }
    
    char operator[](ULong index) const
    {
      return m_ptr[index];
    }
    
  private:
    char* m_ptr;
};

class String_out
{
  public:
    String_out(char*& p) : m_ptr(p) 
    { 
      m_ptr = 0;
    }
    
    String_out(String_var& p) : m_ptr(p.m_ptr)
    {
      string_free(m_ptr); 
      m_ptr = 0;
    }
    
    String_out(const String_out& s) : m_ptr(s.m_ptr) {}
    
    String_out& operator=(const String_out& s) 
    {
      m_ptr = s.m_ptr; 
      return *this;
    }
    
    String_out& operator=(char* p) 
    {
      m_ptr = p; return *this;
    }
    
    String_out& operator=(const char* p) 
    {
      m_ptr = string_dup(p); 
      return *this;
    }
    
    operator char*&() { return m_ptr; }
    operator const char*() const { return m_ptr;}
    char*& ptr() { return m_ptr; }
    
    private:
      char*& m_ptr;
      
      // assignment from String_var disallowed
      void operator=(const String_var&){}
};

} // CORBA

inline ostream& operator<<(ostream& os, const ::CORBA::String_var& s)
{
  os << (const char *) s;
  return os;  
}

inline ostream& operator<<(ostream& os, const ::CORBA::String_out& s)
{
  os << (const char *) s;
  return os;  
}

inline istream& operator>>(istream& is, ::CORBA::String_var& s)
{
  is >> (char *) s;
  return is;  
}

inline istream& operator>>(istream& is, ::CORBA::String_out& s)
{
  is >> (char *) s;
  return is;  
}


#endif
