/////////////////////////////////////////////////////////////////////////
//
// File:        CompletionWaiter.C
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

#include "TIDorb/core/poa/CompletionWaiter.h"
//MLG
//TIDorb::core::poa::CompletionWaiter::CompletionWaiter(TIDorb::core::TIDORB* orb) throw (TIDThr::SystemException) {
TIDorb::core::poa::CompletionWaiter::CompletionWaiter(TIDorb::core::TIDORB* orb) {
//EMLG
        _orb = orb;
        _active_requests = 0;
        _stopWaiting = false;
};
 
//MLG 
//TIDorb::core::poa::CompletionWaiter::~CompletionWaiter() throw(TIDThr::SystemException) {
TIDorb::core::poa::CompletionWaiter::~CompletionWaiter() {
//EMLG
        delete _orb;
};

CORBA::Boolean TIDorb::core::poa::CompletionWaiter::conditionToWait() {
        
        try {
                CORBA::Object_var obj = _orb->init_POACurrent();
                CORBA::Object_ptr obj_ptr = obj;
                TIDorb::core::poa::CurrentImpl* current = dynamic_cast<TIDorb::core::poa::CurrentImpl*>(obj_ptr);

                //TIDorb::core::poa::CurrentImpl* current = _orb->init_POACurrent();
                PortableServer::POA_ptr poa_aux = current->get_POA();
                TIDorb::core::poa::POAImpl* poa = dynamic_cast<TIDorb::core::poa::POAImpl*>(poa_aux);
                return poa->orb != _orb;
                
        } catch (const PortableServer::Current::NoContext) {
                return true;
        }
};
  
void TIDorb::core::poa::CompletionWaiter::beginRequest() {
        TIDThr::Synchronized synchro(monitor);
        
        _active_requests++;
};

void TIDorb::core::poa::CompletionWaiter::endRequest() {
        TIDThr::Synchronized synchro(monitor);
        
        _active_requests--;
        if (_active_requests <= 0) {
                monitor.notify();
        }
};

void TIDorb::core::poa::CompletionWaiter::waitForCompletion() {
        TIDThr::Synchronized synchro(monitor);
        
        while ((_active_requests != 0) && !_stopWaiting) {
                try {
                        monitor.wait();
                } catch (const CORBA::Exception) {
                }
        }
        _stopWaiting = false;
};
  
void TIDorb::core::poa::CompletionWaiter::stopWaiting() {
        TIDThr::Synchronized synchro(monitor);
        
        _stopWaiting = true;
        monitor.notifyAll();
};
    
CORBA::ULong TIDorb::core::poa::CompletionWaiter::getActiveRequests() {
        TIDThr::Synchronized synchro(monitor);
        
        return _active_requests;
};
