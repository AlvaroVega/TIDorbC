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
