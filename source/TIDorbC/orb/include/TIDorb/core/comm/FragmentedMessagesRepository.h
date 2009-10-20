/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/FragmentedMessagesRepository.h
  
 Revisions:
 
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
