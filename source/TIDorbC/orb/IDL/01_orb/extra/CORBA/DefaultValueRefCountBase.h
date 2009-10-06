#include "CORBA.h"


#ifndef _CORBA_DEFAULT_VALUE_REF_COUNT_BASE_H_
#define _CORBA_DEFAULT_VALUE_REF_COUNT_BASE_H_


class DefaultValueRefCountBase : public virtual ValueBase
{
  public:

    ValueBase* _add_ref();
    
    void _remove_ref();

    ULong _refcount_value();

    virtual ValueBase* _copy_value();

  protected:
    DefaultValueRefCountBase();
    virtual ~DefaultValueRefCountBase();

    void _count(CORBA::ULong v);

  private:
    // Reference Count. When the reference was 0, the object can be destroyed
    CORBA::ULong __ref_count_;
    pthread_mutex_t __ref_mutex_;
};


#endif
