/////////////////////////////////////////////////////////////////////////
//
// File:        CurrentImpl.h
// Description:
//              
// Rel:         01.00
// Created:     November, 2002
//
// Revised:    
//
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
/////////////////////////////////////////////////////////////////////////

#include "TIDorb/core/poa.h"

#ifndef _TIDORB_CORE_POA_CurrentImpl_H_
#define _TIDORB_CORE_POA_CurrentImpl_H_

#include "TIDThr.h"

namespace TIDorb {
namespace core {
namespace poa {

/**
 * TIDORB implementation of the Current CORBA Object. The current object accesses 
 * to the CurrentInfo stored in the ExecThread, the thread assotiated to the servant 
 * execution, returning at each moment the current poa and
 * object id (if exists). 
 *
 * @see CurrentInfo
 * @see ExecThread
 *
*/

class CurrentImpl : public PortableServer::Current,
                    public TIDThr::RefCounter,
                    public CORBA::LocalObject
{
private:

	TIDorb::core::TIDORB* _orb;
protected:
	/**
	 * Looks in the current thread for the <code>CurrentInfo</code> that determines the
	 * current poa and object id needed by the <code>Current</code> object.
	 *
	 * @return the <code>CurrentInfo</code> object if exits.
	 *
	 * @exception org.omg.PortableServer.CurrentPackage.NoContext if the current thread is
	 * not an instance of <code>ExecThread</code> (it is the orb main thread) or it is and 
	 * it is executing a local request, and there is out of a POA-dispatched operation.
	 */ 
	 TIDorb::core::poa::CurrentInfo* getCurrentInfo()
		throw(PortableServer::Current::NoContext);
	
public:
	CurrentImpl(TIDorb::core::TIDORB* orb);
	~CurrentImpl() throw(TIDThr::SystemException);
	
	PortableServer::POA* get_POA()
		throw(PortableServer::Current::NoContext);
		
	PortableServer::ObjectId_ptr  get_object_id() 
  		throw(PortableServer::Current::NoContext);
  
        CORBA::Object_ptr get_reference()
		throw (PortableServer::Current::NoContext);
		 
	PortableServer::Servant get_servant()
    		throw(PortableServer::Current::NoContext);
		
	bool in_context();
};

} //poa
} //core
} //TIDorb
#endif
