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
