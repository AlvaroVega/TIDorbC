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
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
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

