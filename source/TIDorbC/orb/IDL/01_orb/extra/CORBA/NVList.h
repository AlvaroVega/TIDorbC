#include "CORBA.h"


#ifndef _CORBA_NV_LIST_H_
#define _CORBA_NV_LIST_H_


class NVList
{
  public:
    virtual ~NVList() {}
  
    virtual ULong count() const = 0;
    virtual NamedValue_ptr add(Flags flags) = 0;
    virtual NamedValue_ptr add_item(const char* item_name, Flags flags) = 0;
    virtual NamedValue_ptr add_value(const char* item_name, const Any& value, Flags flags) = 0;
    virtual NamedValue_ptr add_item_consume(char* item_name, Flags flags) = 0;

    virtual NamedValue_ptr add_value_consume(char* item_name,Any* value, Flags flags) = 0;

    virtual NamedValue_ptr item(ULong index) = 0;
    virtual void remove(ULong index) = 0;
    
    static NVList_ptr _duplicate(NVList_ptr nv);
    static NVList_ptr _nil();
    void _add_ref() { 	
      __ref_count_++; 
    }
    void _remove_ref() { 
      size_t value = --__ref_count_;
      if(value == 0) delete this;
    }

  protected:
    NVList(){}

    size_t __ref_count_;

//MLG
  public:
    virtual void* _impl() = 0;
    virtual const char* _typeid() = 0;
//EMLG

};


#endif //_CORBA_NV_LIST_H_
