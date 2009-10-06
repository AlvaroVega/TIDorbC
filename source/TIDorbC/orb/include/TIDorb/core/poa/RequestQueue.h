/////////////////////////////////////////////////////////////////////////
//
// File:        RequestQueue.h
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

#ifndef _TIDORB_CORE_POA_RequestQueue_H_
#define _TIDORB_CORE_POA_RequestQueue_H_


/**
 * Queue of requests to be read by the execution thread.
 */

#include "TIDThr.h"

#include <deque>
#include <queue>

#include <math.h>

#ifdef TIDORB_HAVE_NAMESPACE_STD
  using namespace std;
#endif

#ifdef __SUNPRO_LIBRARY_IOSTREAM
  using std::deque;
  using std::queue;
#endif

namespace TIDorb {
namespace core {
namespace poa {


// Strict Weak Ordering: returning true if the first precedes the second

// Function object comparator
template<class T> // templating is optional
struct TemporalRequestComparator{
  bool operator()(const T &request1, const T &request2) {
    CORBA::Long serial1 = request1->getSerial();
    CORBA::Long serial2 = request2->getSerial();
    return serial1 < serial2;
  }
};


template<class T>
struct DeadlineRequestComparator{
  bool operator()(const T &request1, const T &request2) const{

    TIDorb::core::PolicyContext* context1 = request1->getPolicyContext();
    TIDorb::core::PolicyContext* context2 = request2->getPolicyContext();

    CORBA::Policy_ptr  pol1, pol2;
    if (context1 == NULL)
      pol1 = NULL;
    else
      pol1 = context1->getPolicy(Messaging::REQUEST_END_TIME_POLICY_TYPE);

    if (context2 == NULL)
      pol2 = NULL;
    else
      pol2 = context2->getPolicy(Messaging::REQUEST_END_TIME_POLICY_TYPE);
    
    if(pol1 == NULL) {
      if(pol2 == NULL) {
        //return TemporalRequestComparator<QueuedRequest*>::operator()(request1, request2);  
        CORBA::Long serial1 = request1->getSerial();
        CORBA::Long serial2 = request2->getSerial();
        return serial1 < serial2;
      } else {
        return false; //GREATER_THAN;
      }
    } else {
      if(pol2 == NULL) {
        return true; //LESS_THAN;
      } else { // both non-null
        
        Messaging::RequestEndTimePolicy_ptr end1, end2;        
        end1 = Messaging::RequestEndTimePolicy::_narrow(pol1);
        end2 = Messaging::RequestEndTimePolicy::_narrow(pol2);

        TimeBase::TimeT val1 = end1->end_time().time;
        TimeBase::TimeT val2 = end2->end_time().time;
        
        if(val1 > val2) {
          return false; // GREATER_THAN;
        } else if(val1 == val2) {
          // return TemporalRequestComparator<QueuedRequest*>::operator()(request1, request2); 
          CORBA::Long serial1 = request1->getSerial();
          CORBA::Long serial2 = request2->getSerial();
          return serial1 < serial2;
        } else {
          return true; // LESS_THAN;
        }
      }
    }       
  }
};



template<class T>// templating is optional
struct PriorityRequestComparator{
  Messaging::Priority getPriority(Messaging::RequestPriorityPolicy_ptr policy) {
    Messaging::Priority priority = 0; // DEFAULT_PRIORITY;
    if(policy != NULL) {
      Messaging::PriorityRange range = policy->priority_range();
      priority = ceil((float) (range.max + range.min) / 2.0);           
    }
    return priority;        
  }
  bool operator()(const T &request1, const T &request2) const{
    TIDorb::core::PolicyContext* context1 = request1->getPolicyContext();
    TIDorb::core::PolicyContext* context2 = request2->getPolicyContext();

    Messaging::RequestPriorityPolicy_ptr reqPriority1, reqPriority2;
    CORBA::Policy_ptr pol1, pol2;

    if (context1 == NULL) 
      reqPriority1 = NULL;
    else{
      pol1 = context1->getPolicy(Messaging::REQUEST_PRIORITY_POLICY_TYPE);
      reqPriority1 = Messaging::RequestPriorityPolicy::_narrow(pol1);
    }

    if (context2 == NULL) 
      reqPriority2 = NULL;
    else{
      pol2 = context2->getPolicy(Messaging::REQUEST_PRIORITY_POLICY_TYPE);
      reqPriority2 = Messaging::RequestPriorityPolicy::_narrow(pol2);
    }

    Messaging::Priority priority1 = 0, priority2 = 0;
     
    if(reqPriority1 != NULL) {
      Messaging::PriorityRange range = reqPriority1->priority_range();
      priority1 = (Messaging::Priority) ceil((float) (range.max + range.min) / 2.0);           
    }
    if(reqPriority2 != NULL) {
      Messaging::PriorityRange range = reqPriority2->priority_range();
      priority2 = (Messaging::Priority) ceil((float) (range.max + range.min) / 2.0);           
    }
                      
    if(priority1 > priority2) {
      return true; // LESS_THAN;
    } else if(priority1 == priority2) {
      // return TemporalRequestComparator<QueuedRequest*>::operator()(request1, request2);
      CORBA::Long serial1 = request1->getSerial();
      CORBA::Long serial2 = request2->getSerial();
      return serial1 < serial2;
    } else {
      return false; // GREATER_THAN;
    }

  }
};


enum RequestComparatorType{TEMPORAL_REQUEST_COMPARATOR = 0, 
                           PRIORITY_REQUEST_COMPARATOR,
                           DEADLINE_REQUEST_COMPARATOR,
                           NULL_REQUEST_COMPARATOR};


class RequestQueue : public TIDThr::Monitor {
  // Native implementation
private:

  TIDorb::core::poa::QueueReaderManager* _reader;
  TIDorb::core::poa::POAManagerImpl* _poaManager;
  bool _deactivation;
  
  typedef priority_queue<QueuedRequest*, deque<QueuedRequest*>,
                         TemporalRequestComparator<QueuedRequest*> > temporal_queueT;

  typedef priority_queue<QueuedRequest*, deque<QueuedRequest*>,
                         PriorityRequestComparator<QueuedRequest*> > priority_queueT;

  typedef priority_queue<QueuedRequest*, deque<QueuedRequest*>,
                         DeadlineRequestComparator<QueuedRequest*> > deadline_queueT;

  typedef deque<QueuedRequest*> simple_queueT;

  temporal_queueT* _temporal_value;
  priority_queueT* _priority_value;
  deadline_queueT* _deadline_value;
  simple_queueT* _value;

  RequestComparatorType _comparator_type; 
	
public:  
  /**
   * Constructor.
   * @param poaManager POAManager to which this RequestQueue belongs.
   * @param reader Object that manages the creation of new readers for this queue.
   */
  RequestQueue(TIDorb::core::poa::POAManagerImpl* poaManager, 
               TIDorb::core::poa::QueueReaderManager* reader,
               TIDorb::core::poa::RequestComparatorType comparator_type)
    throw (TIDThr::SystemException);
  

  ~RequestQueue()  throw (TIDThr::SystemException);
  
    
  /**
   * Adds an new request to the queue.
   * @param request The request to be added.
   */
  void add(TIDorb::core::poa::QueuedRequest* request);
  
  /**
   * Gets (and removes) the first element of the queue.
   * @return The first element of the queue.
   */
  TIDorb::core::poa::QueuedRequest* get();
  
  /**
   * @return Number of enqueued elements.
   */
  CORBA::ULong size();
    
  /**
   * Set all request to "discarding".
   */
  void discardAll();
  
  /**
   * The POAManager is being deactivating, notify it to all blocked threads.
   */
  void deactivation();
  

  unsigned long _maxAvailableRequests;

  unsigned long _starving_time;  
  
  
  
  /**
   * Set comparator to insert into a queue.
   */
  void setComparator(RequestComparatorType type);

};

} //poa
} //core
} //TIDorb
#endif
