/////////////////////////////////////////////////////////////////////////
//
// File:        FragmentedMessagesRepository.C
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

#include "TIDorb/core/comm.h"

namespace TIDorb {
namespace core {
namespace comm {

FragmentedMessagesRepository::FragmentedMessagesRepository(size_t max_size)
: messages(max_size), destroyed(false) 
{}

FragmentedMessagesRepository::~FragmentedMessagesRepository()
{
    destroy();
}
        
/**
 * Puts a new fragmented message
 */
void FragmentedMessagesRepository::put_message(const MessageId& id,
                 TIDorb::core::comm::iiop::GIOPFragmentedMessage* message)
{
    TIDThr::Synchronized sync(mutex);
    
    if(destroyed) { // discard message
        delete message;
        throw CORBA::INTERNAL("FragmentedMessagesRepository Destroyed");
    }
    
    try {
     messages.append(id, new FragmentedMessageHolder(message));
     
     FragmentedMessages::ObjectList* removed = messages.getRemovedObjects();
     
     if(removed!= NULL)
     {
        FragmentedMessages::ObjectList::iterator it = removed->begin();
                
        while(it != removed->end()) {
            delete (*it);
            it++;
        } 
     }
     
    } catch (const TIDorb::core::util::FullUseTableException& fute) {
      //unreachable: holder always can be removed
    }
}

/**
 * Adds a new fragment to the message.
 * @return the complete message if finished, or NULL otherwise
 */    
TIDorb::core::comm::iiop::GIOPFragmentedMessage*  
    FragmentedMessagesRepository::add_fragment(const MessageId& id,
                  TIDorb::core::comm::iiop::GIOPFragmentMessage* fragment)
{
    TIDThr::Synchronized sync(mutex);
    
    if(destroyed) { // discard message
        delete fragment;
        throw CORBA::INTERNAL("FragmentedMessagesRepository Destroyed");
    }
   
    FragmentedMessageHolder* holder = messages.get(id);
   
    if( (holder) && (holder->get_message())) {                 
        holder->get_message()->add_fragment(fragment);
        if(!fragment->getHeader().hasMoreFragments()) {
           TIDorb::core::comm::iiop::GIOPFragmentedMessage*  message = 
                holder->consume_message();                
           messages.remove(id);
           delete holder;
           return message;
        } else {
            return NULL;
        }        
    } else {
        delete fragment;
        return NULL;
    }
}

void FragmentedMessagesRepository::remove(const MessageId& id)
{
    TIDThr::Synchronized sync(mutex);
    
    FragmentedMessageHolder* holder = messages.get(id);
    
    if((holder)) {
        messages.remove(id);
        delete holder;
    }
    
}  
                      
void FragmentedMessagesRepository::destroy() 
{
    TIDThr::Synchronized sync(mutex);
    
    FragmentedMessages::ObjectList* elements = messages.elements(); 
         
    FragmentedMessages::ObjectList::iterator it = elements->begin();
                
    while(it != elements->end()) {
        delete (*it);
        it++;
    } 
     
    delete elements;
    
    messages.clear();
}

}
}
}

