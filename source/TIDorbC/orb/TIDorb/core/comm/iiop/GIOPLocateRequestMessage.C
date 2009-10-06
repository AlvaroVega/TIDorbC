//////////////////////////////////////////////////////////////////////////////////
//
// File:        GIOPLocateRequestMessage.C
// Description: 
//
// Rel:         01.00
// Created:     September, 2002
// Author:      Juan A. Caceres Exposito      ( caceres@tid.es   )
//
// Revised:
//
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
///////////////////////////////////////////////////////////////////////////////////

#include "TIDorb/core/comm/iiop/GIOPLocateRequestMessage.h"

//MLG
void* TIDorb::core::comm::iiop::GIOPLocateRequestMessage::_impl()
{
	return this;
}

const char* TIDorb::core::comm::iiop::GIOPLocateRequestMessage::_typeid()
{
	//return CORBA::string_dup("GIOPLocateRequestMessage");
	return "GIOPLocateRequestMessage";
}
//EMLG


TIDorb::core::comm::iiop::GIOPLocateRequestMessage::GIOPLocateRequestMessage
  (const TIDorb::core::comm::iiop::GIOPHeader& header)
  : GIOPFragmentedMessage(header)
{
}

TIDorb::core::comm::iiop::GIOPLocateRequestMessage::GIOPLocateRequestMessage
  (const TIDorb::core::comm::iiop::Version& ver,
   TIDorb::core::comm::iiop::RequestId id)
  : GIOPFragmentedMessage(GIOPHeader(ver, LocateRequest), id)
{
}



TIDorb::core::comm::iiop::TargetAddress*
TIDorb::core::comm::iiop::GIOPLocateRequestMessage::extract_address()
{
  if(!_message_completed)
    throw CORBA::INTERNAL("Uncompleted message");

  TIDorb::core::comm::iiop::TargetAddress* target = new TargetAddress();

  if(_header.getVersion() >= Version::VERSION_1_2) {
    // request_id has been readed by father's receive_body
    target->read(*message_buffer_in);

  } else {

    message_buffer_in->read_ulong(_request_id);
    TIDorb::core::comm::iiop::ObjectKey* key = new ObjectKey();
    key->read(*message_buffer_in);
    target->object_key(key); // consumed
  }

  return target;
}




void TIDorb::core::comm::iiop::GIOPLocateRequestMessage::insert_address
    (TIDorb::core::TIDORB* orb, 
     const TIDorb::core::comm::iiop::ObjectKey& key)
{
  TIDorb::core::comm::iiop::GIOPFragmentedMessage::create_message_buffer_output(orb);

  if(_header.getVersion() >= Version::VERSION_1_2) {
    TargetAddress::write_object_key_address(*message_buffer_out, key);
  } else {
    message_buffer_out->write_ulong(_request_id);
    key.write(*message_buffer_out);
  }

  _message_completed = true;
}

void TIDorb::core::comm::iiop::GIOPLocateRequestMessage::insert_address
    (TIDorb::core::TIDORB* orb, 
     const TIDorb::core::iop::IOR& ior,
     AddressingDisposition disposition)
    
{
  TIDorb::core::comm::iiop::GIOPFragmentedMessage::create_message_buffer_output(orb);

  if(_header.getVersion() >= Version::VERSION_1_2) {
    switch(disposition) {
        case KeyAddr:
            TargetAddress::write_object_key_address(*message_buffer_out, ior);
            break;
        case ReferenceAddr:
            TargetAddress::write_ior_address(*message_buffer_out, ior);
            break;
        case ProfileAddr:
            TargetAddress::write_profile_address(*message_buffer_out, ior);
    }
  } else {

    message_buffer_out->write_ulong(_request_id);
    TIDorb::core::comm::iiop::ObjectKey* key = ior.object_key();

    if (key == NULL)
      throw CORBA::INTERNAL(); //"Invalid Object Key");
      
    key->write(*message_buffer_out);
  }

  _message_completed = true;
}
