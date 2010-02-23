#include "PortableServer.h"


#ifndef _PORTABLE_SERVER_SERVANT_BASE_H_
#define _PORTABLE_SERVER_SERVANT_BASE_H_


class ServantBase : public virtual TIDorb::portable::RefCounter
{
  public:
    virtual ~ServantBase();
    virtual POA_ptr _default_POA();
    virtual CORBA::InterfaceDef_ptr _get_interface();
    virtual CORBA::Boolean _is_a(const char* logical_type_id);
    virtual CORBA::Boolean _non_existent();
    
    // Reference counting inherited from RefCounter 
    // virtual void _add_ref();
    // virtual void _remove_ref();
    // virtual CORBA::ULong _refcount_value();

    TIDorb::portable::ServantDelegate* get_delegate();
    void set_delegate(TIDorb::portable::ServantDelegate* delegate);
    
    // TIDorb auxiliar operation
    
    virtual const CORBA::RepositoryIdSeq_ptr 
      _all_interfaces(PortableServer::POA_ptr poa, 
                      const PortableServer::ObjectId& objectId);
     
  protected:
    ServantBase();
    ServantBase(const ServantBase &);
    ServantBase& operator=(const ServantBase&);

  private:
    TIDorb::portable::ServantDelegate* _delegate;
};

struct RefCountServantBase {}; //deprecated


#endif
