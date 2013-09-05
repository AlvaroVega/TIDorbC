#include "CORBA.h"


#ifndef _CORBA_VALUE_FACTORY_H_
#define _CORBA_VALUE_FACTORY_H_


class ValueFactoryBase: public virtual TIDorb::portable::RefCounter
{
  friend class TIDorb::portable::InputStream;

  public:
    virtual ~ValueFactoryBase() {}

    static ValueFactory _downcast(ValueFactory vf)
    {
      return dynamic_cast<ValueFactory> (vf);
    }

  protected:
    ValueFactoryBase() {}

  private:
    virtual ValueBase* create_for_unmarshal() = 0;
};

#endif
