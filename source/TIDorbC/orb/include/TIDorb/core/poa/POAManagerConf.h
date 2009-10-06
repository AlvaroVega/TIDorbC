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
