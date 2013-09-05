#include "CORBA.h"

#ifndef _CORBA_ANY_H_
#define _CORBA_ANY_H_

  
class Any
{
  public:
    Any();
    Any(const Any& value);
    ~Any();
    
    Any& operator=(const Any& value);
     
    bool operator==(const Any& value) const;
    bool operator!=(const Any& value) const;
    
    // special types needed for boolean, octet, char,
    // and bounded string insertion
    // these are suggested implementations only

    struct from_boolean {
      from_boolean(Boolean b) : val(b) {}
      Boolean val;
    };
    
    struct from_octet {
      from_octet(Octet o) : val(o) {}
      Octet val;
    };
    
    struct from_char {
      from_char(Char c) : val(c) {}
      Char val;
    };
    
    struct from_wchar {
      from_wchar(WChar c) : val(c) {}
      WChar val;
    };
    
    struct from_string {
      from_string(char* s, ULong b, Boolean n = false) : val(s), bound(b), nocopy(n) {}
      from_string(const char* s, ULong b) : val(const_cast<char*>(s)), bound(b), nocopy(false) {}
      
      char *val;
      ULong bound;
      Boolean nocopy;
    };
    
    struct from_wstring {
      from_wstring(WChar* s, ULong b,  Boolean n = false) : val(s), bound(b), nocopy(n) {}
      from_wstring(const WChar* s, ULong b) : val(const_cast<WChar*>(s)), bound(b), nocopy(false) {}
      
      WChar *val;
      ULong bound;
      Boolean nocopy;
    };
    
    struct from_fixed {
      from_fixed(const Fixed& f, UShort d, UShort s) : val(f), digits(d), scale(s) {}
    
      const Fixed& val;
      UShort digits;
      UShort scale;
    };
    
    void operator<<=(from_boolean value);
    void operator<<=(from_char value);
    void operator<<=(from_wchar value);
    void operator<<=(from_octet value);
    void operator<<=(from_string value);
    void operator<<=(from_wstring value);
    void operator<<=(from_fixed value);
    void operator<<=(Short value);
    void operator<<=(UShort value);
    void operator<<=(Long value);
    void operator<<=(ULong value);
    void operator<<=(Float value);
    void operator<<=(Double value);
    void operator<<=(LongLong value);
    void operator<<=(ULongLong value);
    void operator<<=(LongDouble value);
    void operator<<=(const Any& value); // copying
    void operator<<=(Any* value); // non-copying
    void operator<<=(const char* value);
    void operator<<=(const WChar* value);
    void operator<<=(CORBA::Object_ptr value);    
    void operator<<=(CORBA::TypeCode_ptr value);  
        
    // special types needed for boolean, octet,
    // char extraction
    // these are suggested implementations only
    struct to_boolean {
      to_boolean(Boolean& b) : ref(b) {}
      Boolean& ref;
    };
    struct to_char {
      to_char(Char& c) : ref(c) {}
      Char& ref;
    };
    struct to_wchar {
      to_wchar(WChar& c) : ref(c) {}
      WChar& ref;
    };
    struct to_octet {
      to_octet(Octet& o) : ref(o) {}
      Octet& ref;
    };
    
    struct to_object {
      to_object(Object_out obj) : ref((Object_ptr&) obj) {}
      Object_ptr& ref;
    };
    struct to_string {
      to_string(const char*& s, ULong b): val(s), bound(b) {}
      const char*& val;
      ULong bound;
     };
    
    struct to_wstring {
      to_wstring(const WChar*& s, ULong b): val(s), bound(b) {}
      const WChar*& val;
      ULong bound;
    };
    
    struct to_fixed {
      to_fixed(Fixed& f, UShort d, UShort s) : ref(f), digits(d), scale(s) {}
      Fixed& ref;
      UShort digits;
      UShort scale;
    };
    
    struct to_abstract_base {
      to_abstract_base(AbstractBase_ptr& base) : ref(base) {}
      AbstractBase_ptr& ref;
    };
    
    struct to_value {
      to_value(ValueBase*& base) : ref(base) {}
      ValueBase*& ref;
    };
    
    Boolean operator>>=(to_boolean value) const;
    Boolean operator>>=(to_char value) const;
    Boolean operator>>=(to_wchar value) const;
    Boolean operator>>=(to_octet value) const;
    Boolean operator>>=(to_object value) const;
    Boolean operator>>=(to_string value) const;
    Boolean operator>>=(to_wstring value) const;
    Boolean operator>>=(to_fixed value) const;
    Boolean operator>>=(to_abstract_base value) const;
    Boolean operator>>=(to_value value) const;
    Boolean operator>>=(Short& value) const;
    Boolean operator>>=(UShort& value) const;
    Boolean operator>>=(Long& value) const;
    Boolean operator>>=(ULong& value) const;
    Boolean operator>>=(Float& value) const;
    Boolean operator>>=(Double& value) const;
    Boolean operator>>=(LongLong& value) const;
    Boolean operator>>=(ULongLong& value) const;
    Boolean operator>>=(LongDouble& value) const;
    Boolean operator>>=(const Any*& value) const;
    Boolean operator>>=(const char*& value) const;
    Boolean operator>>=(const WChar*& value) const;
    Boolean operator>>=(TypeCode_ptr& value) const;
     
    TypeCode_ptr type() const;
    
    void type(TypeCode_ptr);
   
    
    //TIDorb
    TIDorb::portable::Any& delegate();  
    TIDorb::portable::Any& delegate() const;
    
  private:
    
    TIDorb::portable::Any* m_delegate;
    
    // these are hidden and should not be implemented
    // so as to catch erroneous attempts to insert
    // or extract multiple IDL types mapped to unsigned char
    
    void operator<<=(unsigned char){}
    Boolean operator>>=(unsigned char&) const{return false;}
};


#endif
