/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/FragmentedMessagesRepository.h
  
 Revisions:
 
 Copyright 2000 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
 The copyright to the software program(s) is property of Telefonica I+D.
 The program(s) may be used and or copied only with the express written
 consent of Telefonica I+D or in acordance with the terms and conditions
 stipulated in the agreement/contract under which the program(s) have
 been supplied.   
 ------------------------------------------------------------------------------ */

#ifndef _FRAGMENTED_MESSAGE_REPOSITORY_H_
#define _FRAGMENTED_MESSAGE_REPOSITORY_H_

#include "TIDThr.h"
#include "TIDorb/core/comm/FragmentedMessageHolder.h"


namespace TIDorb {
namespace core {
namespace comm {
    
class MessageId {
    public:
        const Connection* connection;
        TIDorb::core::comm::iiop::RequestId request_id;
        
       MessageId(const Connection* conn,
                 TIDorb::core::comm::iiop::RequestId id)
        : connection(conn), request_id(id) {}
        
       MessageId(const MessageId& id)
        : connection(id.connection), request_id(id.request_id) {}
    
       bool operator<(const MessageId& id) const
       {
         if(request_id < id.request_id){
            return true;
         } else {
            return (connection < id.connection);
         }
       }
       
       bool operator== (const MessageId& id) const 
       {
         return ((request_id == id.request_id) &&
                (connection == id.connection));
       }
       
       bool operator>(const MessageId& id) const
       {
         if(request_id > id.request_id){
            return true;
         } else {            
            return (connection > id.connection);
         }
       }    
};
    
class FragmentedMessagesRepository {
    
    public:
        
        FragmentedMessagesRepository(size_t max_size);
        ~FragmentedMessagesRepository();
        
        /**
         * Puts a new fragmented message
         */
        void put_message(const MessageId& id,
                         TIDorb::core::comm::iiop::GIOPFragmentedMessage* message);
        
        /**
         * Remove the message from the list a fragmented message
         */
         
         void remove(const MessageId& id);            
        
        
        /**
         * Adds a new fragment to the message.
         * @return the complete message if completed, or NULL otherwise
         */   
        TIDorb::core::comm::iiop::GIOPFragmentedMessage* 
             add_fragment(const MessageId& id,
                          TIDorb::core::comm::iiop::GIOPFragmentMessage* fragment);
        /**
         * Clears the list
         */
         
        void destroy();
        
    
    protected:
    
        typedef TIDorb::core::util::UseTable<MessageId,
                                     FragmentedMessageHolder> FragmentedMessages;
                                     
        FragmentedMessages messages;
        
        bool destroyed;
        
        TIDThr::Mutex mutex;
        
        
};


}
}
}
    
#endif //_FRAGMENTEDMESSAGEREPOSITORY_H_
