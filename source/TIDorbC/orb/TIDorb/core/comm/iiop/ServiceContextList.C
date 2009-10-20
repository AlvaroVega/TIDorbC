//////////////////////////////////////////////////////////////////////////////////
//
// File:        ServiceContextList.C
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

#include "TIDorb/core/comm/iiop/ServiceContextList.h"

TIDorb::core::comm::iiop::ServiceContextList::ServiceContextList()
{
}

TIDorb::core::comm::iiop::ServiceContextList::ServiceContextList(CORBA::ULong count)
{
  //components.resize(count); // core
};

TIDorb::core::comm::iiop::ServiceContextList::~ServiceContextList()
{
  components.clear();
}


//
// void TIDorb::core::comm::iiop::ServiceContextList::add(
//                                          TIDorb::core::comm::iiop::ServiceContext* context)
// {
//   components.push_back(context);
// }
	
TIDorb::core::comm::iiop::ServiceContextList* 
TIDorb::core::comm::iiop::ServiceContextList::read(TIDorb::core::cdr::CDRInputStream& input)
{
  CORBA::ULong count;
  input.read_ulong(count);
  
  if(count == 0)
    return NULL;
  
  TIDorb::core::comm::iiop::ServiceContextList* service_context_list = 
    new TIDorb::core::comm::iiop::ServiceContextList(count);
  
  for (CORBA::ULong i = 0; i < count; i++) 
    (service_context_list->components).push_back(
                          TIDorb::core::comm::iiop::ServiceContextReader::read(input));
  
  return service_context_list;
};


void TIDorb::core::comm::iiop::ServiceContextList::write(
                 const TIDorb::core::comm::iiop::ServiceContextList* service_context_list,
                 TIDorb::core::cdr::CDROutputStream& output)
{
  if (service_context_list == NULL)
    output.write_ulong(0);
  else {	
    output.write_ulong(service_context_list->components.size()); 
    for (CORBA::ULong i = 0; i< service_context_list->components.size(); i++)
      (service_context_list->components)[i]->write(output);
  }
};

void TIDorb::core::comm::iiop::ServiceContextList::skip(TIDorb::core::cdr::CDRInputStream& input)
{
  CORBA::ULong count;
  input.read_ulong(count);
  
  for (CORBA::ULong i = 0; i < count; i++)
    TIDorb::core::comm::iiop::ServiceContext::skip(input);
};

