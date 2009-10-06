#include "CORBA.h"


#ifndef _CORBA_CONTEXT_H_
#define _CORBA_CONTEXT_H_


class Context
{
  public:
    virtual ~Context(){}
  
    virtual const char *context_name() const = 0;
    virtual Context_ptr parent() const = 0;
    virtual void create_child(const char* child_context_name, Context_out child_context) = 0;
    virtual void set_one_value(const char* prop_name, const Any& value) = 0;
    virtual void set_values(NVList_ptr values) = 0;
    virtual void delete_values(const char* prop_name) = 0;
    virtual void get_values(const char* prop_name, Flags flags, 
                            const char* patter, NVList_out values) = 0;
    
    static Context_ptr _duplicate(Context_ptr ctx);
    static Context_ptr _nil();
    
  protected:
    Context(){}
};

#endif
