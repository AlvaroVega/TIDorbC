/////////////////////////////////////////////////////////////////////////
//
// File:        ExecThread.h
// Description:
//              
// Rel:         01.00
// Created:     November, 2002
//
// Revised:    
//
// (C) Copyright 2009 Telefonica Investigacion y Desarrollo
//     S.A.Unipersonal (Telefonica I+D)
//
// This file is part of Morfeo CORBA Platform.
//
// Morfeo CORBA Platform is free software: you can redistribute it and/or
// modify it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the License,
// or (at your option) any later version.
//
// Morfeo CORBA Platform is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with Morfeo CORBA Platform. If not, see
//
//   http://www.gnu.org/licenses
//
// Info about members and contributors of the MORFEO project
// is available at
//
//   http://morfeo-project.org
//
/////////////////////////////////////////////////////////////////////////

#include "TIDorb/core/poa.h"

#ifndef _TIDORB_CORE_POA_ExecThread_H_
#define _TIDORB_CORE_POA_ExecThread_H_


#include <stack>

#ifdef TIDORB_HAVE_NAMESPACE_STD
  using namespace std;
#endif

#ifdef __SUNPRO_LIBRARY_IOSTREAM
  using std::stack;
#endif


#include "TIDThr.h"

namespace TIDorb {
namespace core {
namespace poa {

/**
 * Execution thread. Gets request from the request queue and executes them.
*/

class ExecThread : public TIDThr::Thread {
private:  
  TIDorb::core::poa::POAManagerImpl* _poaManager;
  TIDorb::core::poa::RequestQueue* _queue;
  bool _firstTime;
  bool _deactivated;
  TIDorb::core::poa::ThreadStateListener* _threadStateListener;
  stack<TIDorb::core::poa::CurrentInfo*>  _currentInfoStack;
  
  char* _thread_name;
  //  TIDorb::util::Trace* trace;
  
  int _max_blocked_time;
  int _qos_enabled;

  /**
   * Checks the state of the POAManager.
   * If the state is ACTIVE, then finish returning true.
   * If the state is HOLDING, then wait until state is diferent from HOLDING.
   * If the state is DISCARDING, then discard the request and return false.
   * If the state is INACTIVE, then reject the request and return false.
   * @param request The request which is being processed.
   * @return Returns true if the request must be executed, otherwise returns false.
   */
  bool checkState(TIDorb::core::poa::QueuedRequest* request);
    
  /**
   * Process current request.
   * @return <code>true</code> wherther the request has been queued in another 
   * POAManager.
   */
  void processRequest(TIDorb::core::poa::QueuedRequest*& thisRequest);

  /**
   * Finds the POA which corresponds to @param request. If the request
   * belongs to another POAManager it is bypassed to it, returning null.
   * @param request The queued request to be executed.
   * @returns POAImpl POA where the request must be executed.
   * @exception org.omg.PortableServer.POAPackage.AdapterNonExistent If it fails finding the POA.
   */
  TIDorb::core::poa::POAImpl* findPOA(TIDorb::core::poa::QueuedRequest* request)
    throw(PortableServer::POA::AdapterNonExistent, CORBA::SystemException);
  
  /**
   * Checks the state of the given POAManager.
   * If the state is ACTIVE, then finish returning true.
   * If the state is HOLDING, then wait until state changes or the max_blocked_time timeout is reached.
   * If the state is DISCARDING, then discard the request and return false.
   * If the state is INACTIVE, then reject the request and return false.
   * @param request The request which is being processed.
   * @return Returns true if the request must be executed, otherwise returns false.
   */
  bool checkForeingState(TIDorb::core::poa::AbstractLocalRequest* thisRequest,
                         TIDorb::core::poa::POAManagerImpl* current_manager);


public:  
  /**
   * Executes a queued request depending on its type.
   * @param request The queued request to be executed.
   */
  void execute(TIDorb::core::poa::QueuedRequest* request, TIDorb::core::poa::POAImpl* poa);
	
  /**
   * Executes IIOPRequests.
   * @param request The queued request to be executed.
   */
  void execute(TIDorb::core::poa::IIOPRequest* request, TIDorb::core::poa::POAImpl* poa);

  // MIOP extensions

  /**
   * Executes IIOPRequests.
   * @param request The queued request to be executed.
   */
  void execute(TIDorb::core::goa::MIOPRequest* request, TIDorb::core::poa::POAImpl* poa);

  // end MIOP extensions
	
  /**
   * Executes IIOPLocateRequests.
   * @param request The queued request to be executed.
   */
  void execute(TIDorb::core::poa::IIOPLocateRequest* request, TIDorb::core::poa::POAImpl* poa);
    
  /**
   * Executes LocalRequest.
   * @param request The queued request to be executed.
   */
  void execute(TIDorb::core::poa::LocalRequest* localRequest, TIDorb::core::poa::POAImpl* poa);
  
  /**
   * Executes LocalLocateRequest.
   * @param request The queued request to be executed.
   */
  void execute(TIDorb::core::poa::LocalLocateRequest* request, TIDorb::core::poa::POAImpl* poa);
	
  /**
   * Executes "_is_a" request on the ServantDelegate.
   * @param servant The target servant of the "_is_a" request.
   * @param request The queued request to be executed.
   */
private:

  void invoke_is_a(TIDorb::core::poa::POAImpl* poa, PortableServer::ServantBase* servant, CORBA::ServerRequest* request); 
  
  void do_invoke(PortableServer::DynamicImplementation* servant, CORBA::ServerRequest* request);
 
public:

  /**
   * Constructor.
   * @param poaManager POAManager to which this ExecThread belongs.
   * @param number Id number of this ExecThread.
   */
  ExecThread(TIDorb::core::poa::POAManagerImpl* poaManager,
             TIDThr::ThreadGroup* group);
  
  ~ExecThread() throw();

  /**
   * Set the ThreadStateListener. Any ExecThread should have
   * a listener (eventually the ThreadPool) before it is started.
   * @param l The listener.
   */
  void setThreadStateListener(TIDorb::core::poa::ThreadStateListener* l);
	
  /**
   * Execution loop.
   */
  void run();
  	
  /**
   * Process local Request in the given POAManager.
   * @param thisRequest the Local request
   * @param current_manager the current POA POAManager
   */
  void processLocalRequest(TIDorb::core::poa::AbstractLocalRequest* thisRequest,
                           TIDorb::core::poa::POAManagerImpl* current_manager);
  
  /**
   * @return Returns the string representation of this Thread.
   */
  const char* toString();
  
  /**
   * @return Returns the CurrentInfo in the top of the stack.
   */
  TIDorb::core::poa::CurrentInfo* getCurrentInfo();
    
  /**
   * @return Returns true if the request must be executed, otherwise returns false.
   */
  CORBA::Boolean belongsToPOAManager(TIDorb::core::poa::POAManagerImpl* poaManager);
};

} //poa
} //core
} //TIDorb
#endif

