#include "PortableServer.h"


#ifndef _PortableServer_DynamicImplementation_H_
#define _PortableServer_DynamicImplementation_H_


class DynamicImplementation : public virtual PortableServer::ServantBase
{
  public:
    CORBA::Object_ptr _this();

    virtual void invoke(CORBA::ServerRequest_ptr request) = 0;
    virtual CORBA::RepositoryId _primary_interface(const PortableServer::ObjectId& oid,PortableServer::POA_ptr poa) = 0;

}; //class DynamicImplementation


#endif
