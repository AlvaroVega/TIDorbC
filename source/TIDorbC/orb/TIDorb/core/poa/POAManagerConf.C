/////////////////////////////////////////////////////////////////////////
//
// File:        POAManagerConf.C
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

#include "TIDorb/core/poa/POAManagerConf.h"

TIDorb::core::poa::POAManagerConf::POAManagerConf(CORBA::ULong   poa_min_threads, 
                                                  CORBA::ULong   poa_max_threads, 
                                                  CORBA::ULong   poa_max_queued_requests, 
                                                  CORBA::ULong   poa_starving_time, 
                                                  CORBA::ULong   poa_thread_stack_size,
                                                  CORBA::Boolean poa_bound_threads)
{
  min_threads = poa_min_threads;
  max_threads = poa_max_threads;
  max_queued_requests = poa_max_queued_requests;
  starving_time = poa_starving_time;
  thread_stack_size = poa_thread_stack_size;
  bound_threads = poa_bound_threads;
}


TIDorb::core::poa::POAManagerConf::~POAManagerConf()
{
}


void TIDorb::core::poa::POAManagerConf::setListener(TIDorb::core::poa::POAManagerConfListener* l)
{
  listener = l;
}

  
void TIDorb::core::poa::POAManagerConf::setMinThreads(CORBA::ULong poa_min_threads)
{
  TIDThr::Synchronized synchro(recursive_mutex);
	
  if (poa_min_threads < 0) {
    throw CORBA::BAD_PARAM("min_threads cannot be negative.");
  }
  if (poa_min_threads > max_threads) {
    throw CORBA::BAD_PARAM("min_threads cannot be greater than max_threads.");
  }
  min_threads = poa_min_threads;
  if (listener != NULL) {
    listener->minThreadsHasChanged();
  }
}


void TIDorb::core::poa::POAManagerConf::setMaxThreads(CORBA::ULong poa_max_threads)
{
  TIDThr::Synchronized synchro(recursive_mutex);
  
  if (poa_max_threads < min_threads) {
    throw CORBA::BAD_PARAM("max_threads cannot be less than min_threads.");
  }
  max_threads = poa_max_threads;
  if (listener != NULL) {
    listener->maxThreadsHasChanged();
  }
}


void TIDorb::core::poa::POAManagerConf::setMaxQueuedRequests(CORBA::ULong poa_max_queued_requests)
{
  TIDThr::Synchronized synchro(recursive_mutex);
  
  if (poa_max_queued_requests < max_queued_requests) {
    throw CORBA::BAD_PARAM("max_queued_requests cannot be decreased.");
  }
  max_queued_requests = poa_max_queued_requests;
}


void TIDorb::core::poa::POAManagerConf::setStarvingTime(CORBA::ULong millisecs)
{
  TIDThr::Synchronized synchro(recursive_mutex);
  
  if (millisecs < 0) {
    throw CORBA::BAD_PARAM("starving_time cannot be negative.");
  }
  starving_time = millisecs;
}


void TIDorb::core::poa::POAManagerConf::setThreadStackSize(CORBA::ULong bytes)
{
  TIDThr::Synchronized synchro(recursive_mutex);
	
  if (bytes < 0) {
    throw CORBA::BAD_PARAM("stack_size cannot be negative.");
  }
  thread_stack_size = bytes;
}


void TIDorb::core::poa::POAManagerConf::setBoundThread(CORBA::Boolean bound)
{
	
  TIDThr::Synchronized synchro(recursive_mutex);
  
  bound_threads = bound;
}


CORBA::ULong TIDorb::core::poa::POAManagerConf::getMaxQueuedRequests()
{
  TIDThr::Synchronized synchro(recursive_mutex);
  return max_queued_requests;
}

  
CORBA::ULong TIDorb::core::poa::POAManagerConf::getMaxThreads()
{
  TIDThr::Synchronized synchro(recursive_mutex);
  return max_threads;
}


CORBA::ULong TIDorb::core::poa::POAManagerConf::getMinThreads()
{
  TIDThr::Synchronized synchro(recursive_mutex);
  return min_threads;
}


CORBA::ULong TIDorb::core::poa::POAManagerConf::getStarvingTime()
{
  TIDThr::Synchronized synchro(recursive_mutex);
  return starving_time;
}


CORBA::ULong TIDorb::core::poa::POAManagerConf::getThreadStackSize()
{
  TIDThr::Synchronized synchro(recursive_mutex);
  return thread_stack_size;
}


CORBA::Boolean TIDorb::core::poa::POAManagerConf::getBoundThread()
{
  TIDThr::Synchronized synchro(recursive_mutex);
  return bound_threads;
}
