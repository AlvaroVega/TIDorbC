/////////////////////////////////////////////////////////////////////////
//
// File:        POAManagerConf.h
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

#ifndef _TIDORB_CORE_POA_POAManagerConf_H_
#define _TIDORB_CORE_POA_POAManagerConf_H_

#include "TIDThr.h"

/**
 * Configuration set for POAManager.
 */
namespace TIDorb {
namespace core {
namespace poa {

class POAManagerConf
{
    private:
        CORBA::ULong   min_threads;
        CORBA::ULong   max_threads;
        CORBA::ULong   max_queued_requests;
        CORBA::ULong   starving_time;
        CORBA::ULong   thread_stack_size;
        CORBA::Boolean bound_threads;

        TIDorb::core::poa::POAManagerConfListener* listener;
        TIDThr::RecursiveMutex recursive_mutex;                            

    public:
        POAManagerConf(CORBA::ULong min_threads, CORBA::ULong max_threads,
                       CORBA::ULong max_queued_requests, CORBA::ULong starving_time,
                       CORBA::ULong thread_stack_size, CORBA::Boolean bound_threads);
        ~POAManagerConf();

        void setListener(TIDorb::core::poa::POAManagerConfListener* l);

        void setMinThreads(CORBA::ULong min_threads);
        void setMaxThreads(CORBA::ULong max_threads);
        void setMaxQueuedRequests(CORBA::ULong max_queued_requests);
        void setStarvingTime(CORBA::ULong millisecs);
        void setThreadStackSize(CORBA::ULong bytes);
        void setBoundThread(CORBA::Boolean bound);

        CORBA::ULong getMaxQueuedRequests();
        CORBA::ULong getMaxThreads();
        CORBA::ULong getMinThreads();
        CORBA::ULong getStarvingTime();
        CORBA::ULong getThreadStackSize();
        CORBA::Boolean getBoundThread();

};

} //poa
} //core
} //TIDorb

#endif
