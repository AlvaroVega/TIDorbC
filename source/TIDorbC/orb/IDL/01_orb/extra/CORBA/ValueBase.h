#include "CORBA.h"


#ifndef _CORBA_VALUE_BASE_H_
#define _CORBA_VALUE_BASE_H_


class ValueBase
{
  public:
  
    virtual ValueBase* _add_ref() = 0;
    virtual void _remove_ref() = 0;
    virtual ValueBase* _copy_value() = 0;
    virtual ULong _refcount_value() = 0;

    // TIDorb marshalling methods
    virtual void _read(TIDorb::portable::InputStream& input);
    virtual void _write(TIDorb::portable::OutputStream& out) const;
    virtual CORBA::TypeCode_ptr _type() const = 0;
    
    static ValueBase* _downcast(ValueBase* value)
    {
      return dynamic_cast<ValueBase*>(value);
    }
  
  protected:
    ValueBase(){}
    ValueBase(const ValueBase&){}
    virtual ~ValueBase(){}
  private:
    void operator=(const ValueBase&);
};

#endif
