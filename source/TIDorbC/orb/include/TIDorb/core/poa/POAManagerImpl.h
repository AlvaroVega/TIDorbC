/////////////////////////////////////////////////////////////////////////
//
// File:        POAManagerImpl.h
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

#ifndef _TIDORB_CORE_POA_POAManagerImpl_H_
#define _TIDORB_CORE_POA_POAManagerImpl_H_

/**
 * Implementation of POAManager.
 * @version 1.0
*/
#include "TIDorb/core/util.h"


#include <vector>

#ifdef TIDORB_HAVE_NAMESPACE_STD
  using namespace std;
#endif

#ifdef __SUNPRO_LIBRARY_IOSTREAM
  using std::vector;
#endif


namespace TIDorb {
namespace core {
namespace poa {


class POAManagerImpl : public PortableServer::POAManager,
                       public CORBA::LocalObject
{
protected:
  bool  destroyed;
  char* _manager_name;
  Messaging::Ordering m_queue_order;

private:
  static TIDorb::core::util::Counter   _serial;

private:
  TIDThr::Monitor                      monitor;

  vector<PortableServer::POA_var>*     _poas;
  TIDorb::core::poa::ThreadPool*       _pool;
  TIDorb::core::poa::RequestQueue*     _requestQueue;
  PortableServer::POAManager::State    _state;
  TIDorb::core::poa::CompletionWaiter  _completion;

public:
  TIDThr::Monitor                      stateMutex;
  TIDorb::core::TIDORB*                orb;
  TIDorb::core::poa::POAManagerConf*   conf;

  /**
   * Constructor.
   * @param orb The ORB.
   */
  POAManagerImpl(TIDorb::core::TIDORB* the_orb);

  ~POAManagerImpl();

protected:
  /**
   * @return The associated POAManagerConf object.
   */
  TIDorb::core::poa::POAManagerConf* getConf();

  /**
   * Finds a POA in this POA manager.
   * @param poaName The POA name.
   */
  PortableServer::POA_ptr findPOA(const char* poaName);

public:

  const char* toString();

  /**
   * @return The request queue of this POAManager.
   */
  TIDorb::core::poa::RequestQueue* getRequestQueue();

  /**
   * Adds a POA to this POAManager.
   * @param poa The POA.
   */
  void addPOA(PortableServer::POA_ptr poa);

  /**
   * Removes a POA from this POA manager.
   * @param poa The POA being removed.
   */
  void removePOA(PortableServer::POA_ptr poa);

  /**
   * Begins the execution of a request.
   */
  void beginRequest();

  /**
   * Ends the execution of a request.
   */
  void endRequest();

  /**
   * Etherealizes all POAs (due to POA manager deactivation).
   */
  void etherealizeAllPOAs();

  /**
   * Sets the mininum number of execution threads
   */
  void set_min_threads(CORBA::ULong min_threads);

  /**
   * Sets the maximum number of execution threads
   */
  void set_max_threads(CORBA::ULong max_threads);

  /**
   * Sets the maximum number of request queued in the POAManager
   */
  void set_max_queued_requests(CORBA::ULong max_queued_requests);

  /**
   * Sets the maximum time an execution thread is inactive.
   */
  void set_starving_time(CORBA::ULong millisecs);

//MLG
  /**
   * @return The thread pool of this POAManager.
   */
  TIDorb::core::poa::ThreadPool* getThreadPool();
//EMLG

  /**
   * @return maximum number of request queued in the POAManager
   */
  CORBA::ULong get_max_queued_requests();

  /**
   * @return the maximum number of execution threads
   */
  CORBA::ULong get_max_threads();

  /**
   * @return the mininum number of execution threads
   */
  CORBA::ULong get_min_threads();

  /**
   * @return the maximum time an execution thread is inactive.
   */
  CORBA::ULong get_starving_time();

  /* ************ State changes ************* */

  /**
   * This operation changes the POA manager to ACTIVE.
   * @exception org.omg.PortableServer.POAManagerPackage.AdapterInactive  If POA manager state is INACTIVE.
   */
  void activate()
    throw(PortableServer::POAManager::AdapterInactive, CORBA::SystemException);

  /**
   * This operation changes the POA manager to HOLDING.
   * @param wait_for_completion Wait-for-completion flag.
   * @exception org.omg.PortableServer.POAManagerPackage.AdapterInactive  If POA manager state is INACTIVE.
   */
  void hold_requests(bool wait_for_completion)
    throw(PortableServer::POAManager::AdapterInactive, CORBA::SystemException);

  /**
   * This operation changes the POA manager to DISCARDING.
   * @param wait_for_completion Wait-for-completion flag.
   * @exception org.omg.PortableServer.POAManagerPackage.AdapterInactive  If POA manager state is INACTIVE.
   */
  void discard_requests(bool wait_for_completion)
   throw(PortableServer::POAManager::AdapterInactive, CORBA::SystemException);

  /**
   * This operation changes the POA manager to DEACTIVATE.
   * @param etherealize_object If it is true, then all objects must be etherealized.
   * @param wait_for_completion Wait-for-completion flag.
   * @exception org.omg.PortableServer.POAManagerPackage.AdapterInactive  If POA manager state is INACTIVE.
   */
  void deactivate(bool etherealize_objects, bool wait_for_completion)
    throw(PortableServer::POAManager::AdapterInactive, CORBA::SystemException);

  /**
   * @return Return the state of the POA manager.
   */
  PortableServer::POAManager::State get_state()
    throw();

  /* ************ TIDorb methods - POA Location ************* */

  /**
   * Puts a new request in this POA manager.
   * @param request The request.
   */
  void put(TIDorb::core::poa::QueuedRequest* request);


  /**
   * Class to etherealize all POAs in background.
   */
  class EtherealizerThread : public virtual TIDThr::Thread
  {
    public:
      TIDorb::core::poa::POAManagerImpl* poaManager;

      EtherealizerThread(TIDorb::core::poa::POAManagerImpl* the_poaManager);

      void run();
  };
 

public:

  virtual void* _impl();
  virtual const char* _typeid();

  void setQueueOrderPolicy(CORBA::Policy_ptr policy);

  void set_queue_order(Messaging::Ordering order);

  Messaging::Ordering get_queue_order();

};

} //poa
} //core
} //TIDorb

#endif
