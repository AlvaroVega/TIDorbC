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
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
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

