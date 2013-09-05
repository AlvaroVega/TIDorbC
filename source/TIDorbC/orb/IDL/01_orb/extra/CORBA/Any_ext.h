#include "CORBA.h"

#ifndef _CORBA_ANY_EXT_H_
#define _CORBA_ANY_EXT_H_


#include "CORBA/basic_types.h"


inline TIDorb::portable::Any& CORBA::Any::delegate()
{
  return *m_delegate;
}  

inline TIDorb::portable::Any& CORBA::Any::delegate() const
{
  return *m_delegate;
}

inline CORBA::TypeCode_ptr CORBA::Any::type() const
{
  return m_delegate->type();
}
    
inline void CORBA::Any::type(CORBA::TypeCode_ptr tc)
{
  m_delegate->type(tc);
}    

inline void CORBA::Any::operator<<=(CORBA::Any::from_boolean value)
{
  m_delegate->insert_boolean(value.val);
}

inline void CORBA::Any::operator<<=(CORBA::Any::from_char value)
{
  m_delegate->insert_char(value.val);
}

inline void CORBA::Any::operator<<=(CORBA::Any::from_wchar value)
{
  m_delegate->insert_wchar(value.val);
}

inline void CORBA::Any::operator<<=(CORBA::Any::from_octet value)
{
  m_delegate->insert_octet(value.val);
}

inline void CORBA::Any::operator<<=(CORBA::Any::from_string value)
{
//FRAN
  if(value.nocopy){
    m_delegate->insert_string(value.val);
  }
  else{
    m_delegate->insert_string(CORBA::string_dup(value.val));
  }
//EFRAN
}

inline void CORBA::Any::operator<<=(CORBA::Any::from_wstring value)
{
//FRAN
  if(value.nocopy){
    m_delegate->insert_wstring(value.val);
  }
  else{
    m_delegate->insert_wstring(CORBA::wstring_dup(value.val));
  }
//EFRAN
}

inline void CORBA::Any::operator<<=(CORBA::Any::from_fixed value)
{
  m_delegate->insert_fixed(value.val, value.digits, value.scale);
}

inline void CORBA::Any::operator<<=(CORBA::Short value)
{
  m_delegate->insert_short(value);
}

inline void CORBA::Any::operator<<=(CORBA::UShort value)
{
  m_delegate->insert_ushort(value);
}

inline void CORBA::Any::operator<<=(CORBA::Long value)
{
  m_delegate->insert_long(value);
}

inline void CORBA::Any::operator<<=(CORBA::ULong value)
{
  m_delegate->insert_ulong(value);
}

inline void CORBA::Any::operator<<=(CORBA::Float value)
{
  m_delegate->insert_float(value);
}

inline void CORBA::Any::operator<<=(CORBA::Double value)
{
  m_delegate->insert_double(value);
}

inline void CORBA::Any::operator<<=(CORBA::LongLong value)
{
  m_delegate->insert_longlong(value);
}

inline void CORBA::Any::operator<<=(CORBA::ULongLong value)
{
  m_delegate->insert_ulonglong(value);
}

inline void CORBA::Any::operator<<=(CORBA::LongDouble value)
{
  m_delegate->insert_longdouble(value);
}

inline void CORBA::Any::operator<<=(const CORBA::Any& value) // copying
{
  m_delegate->insert_any(value);
}

inline void CORBA::Any::operator<<=(CORBA::Any* value) // non-copying
{
  m_delegate->insert_any(value);
}

inline void CORBA::Any::operator<<=(const char* value)
{
  m_delegate->insert_string(value);
}

inline void CORBA::Any::operator<<=(const CORBA::WChar* value)
{
  m_delegate->insert_wstring(value);
}

inline void CORBA::Any::operator<<=(CORBA::Object_ptr value)
{
  m_delegate->insert_Object(value);
}
   
inline void CORBA::Any::operator<<=(CORBA::TypeCode_ptr value)
{
  m_delegate->insert_TypeCode(value);
}

inline CORBA::Boolean CORBA::Any::operator>>=(to_boolean value) const
{
  return m_delegate->extract_boolean(value.ref);
}

inline CORBA::Boolean CORBA::Any::operator>>=(CORBA::Any::to_char value) const
{
  return m_delegate->extract_char(value.ref);
}

inline CORBA::Boolean CORBA::Any::operator>>=(CORBA::Any::to_wchar value) const
{
  return m_delegate->extract_wchar(value.ref);
}

inline CORBA::Boolean CORBA::Any::operator>>=(CORBA::Any::to_octet value) const
{
  return m_delegate->extract_octet(value.ref);
}
inline CORBA::Boolean CORBA::Any::operator>>=(CORBA::Any::to_object value) const
{
  return m_delegate->extract_Object(value.ref);
}

inline CORBA::Boolean CORBA::Any::operator>>=(CORBA::Any::to_string value) const
{
  return m_delegate->extract_string(value.val);
}

inline CORBA::Boolean CORBA::Any::operator>>=(CORBA::Any::to_wstring value) const
{
  return m_delegate->extract_wstring(value.val);
}

inline CORBA::Boolean CORBA::Any::operator>>=(CORBA::Any::to_fixed value) const
{
  return m_delegate->extract_fixed(value.ref, value.digits, value.scale);
}

inline CORBA::Boolean CORBA::Any::operator>>=(CORBA::Any::to_value value) const
{
  return m_delegate->extract_Value(value.ref);
}

inline CORBA::Boolean CORBA::Any::operator>>=(CORBA::Short& value) const
{
  return m_delegate->extract_short(value);
}

inline CORBA::Boolean CORBA::Any::operator>>=(CORBA::UShort& value) const
{
  return m_delegate->extract_ushort(value);
}

inline CORBA::Boolean CORBA::Any::operator>>=(CORBA::Long& value) const
{
  return m_delegate->extract_long(value);
}

inline CORBA::Boolean CORBA::Any::operator>>=(CORBA::ULong& value) const
{
  return m_delegate->extract_ulong(value);
}

inline CORBA::Boolean CORBA::Any::operator>>=(CORBA::Float& value) const
{
  return m_delegate->extract_float(value);
}

inline CORBA::Boolean CORBA::Any::operator>>=(CORBA::Double& value) const
{
  return m_delegate->extract_double(value);
}

inline CORBA::Boolean CORBA::Any::operator>>=(CORBA::LongLong& value) const
{
  return m_delegate->extract_longlong(value);
}

inline CORBA::Boolean CORBA::Any::operator>>=(CORBA::ULongLong& value) const
{
  return m_delegate->extract_ulonglong(value);
}

inline CORBA::Boolean CORBA::Any::operator>>=(CORBA::LongDouble& value) const
{
  return m_delegate->extract_longdouble(value);
}

inline CORBA::Boolean CORBA::Any::operator>>=(const CORBA::Any*& value) const
{
  return m_delegate->extract_any(value);
}

inline CORBA::Boolean CORBA::Any::operator>>=(const char*& value) const
{
  return m_delegate->extract_string(value);
}

inline CORBA::Boolean CORBA::Any::operator>>=(const CORBA::WChar*& value) const
{
  return m_delegate->extract_wstring(value);
}

inline CORBA::Boolean CORBA::Any::operator>>=(CORBA::TypeCode_ptr& value) const
{
  return m_delegate->extract_TypeCode(const_cast<CORBA::TypeCode_ptr&>(value));
}


#endif //Any_EXT_H
