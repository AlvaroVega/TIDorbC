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
///////////////////////////////////////////////////////////////////////////////////

#include "TIDorb/core/comm/iiop/GIOPLocateRequestMessage.h"

void* TIDorb::core::comm::iiop::GIOPLocateRequestMessage::_impl()
{
	return this;
}

const char* TIDorb::core::comm::iiop::GIOPLocateRequestMessage::_typeid()
{
	//return CORBA::string_dup("GIOPLocateRequestMessage");
	return "GIOPLocateRequestMessage";
}


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
