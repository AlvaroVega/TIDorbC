//////////////////////////////////////////////////////////////////////////////////
//
// File:        RequestId.C
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

#include "TIDorb/core/comm/iiop/RequestId.h"




// TIDorb::core::comm::iiop::RequestIdHolder::RequestIdHolder()
// {
//   _value = 0;
// }	



	
// TIDorb::core::comm::iiop::RequestIdHolder::RequestIdHolder(TIDorb::core::comm::iiop::RequestId val)
// {
//   _value = val;
// }




// TIDorb::core::comm::iiop::RequestIdHolder::RequestIdHolder
//   (const TIDorb::core::comm::iiop::RequestIdHolder& val)
// {
//   _value = val._value;
// }




// TIDorb::core::comm::iiop::RequestIdHolder& 
// TIDorb::core::comm::iiop::RequestIdHolder::operator=(const TIDorb::core::comm::iiop::RequestIdHolder& val)
// {
//   _value = val._value;
//   return *this;
// }




// TIDorb::core::comm::iiop::RequestIdHolder& 
// TIDorb::core::comm::iiop::RequestIdHolder::operator=(TIDorb::core::comm::iiop::RequestId val)
// {
//   _value = val;
//   return *this;
// }



	
// CORBA::ULong TIDorb::core::comm::iiop::RequestIdHolder::value() const
// {
//   return _value;
// }



	
// CORBA::ULong TIDorb::core::comm::iiop::RequestIdHolder::hashCode() const
// {
//   return _value;
// }



	
// bool TIDorb::core::comm::iiop::RequestIdHolder::operator==(const TIDorb::core::comm::iiop::RequestIdHolder& val) const 
// {
//   return _value == val._value;
// }




// bool TIDorb::core::comm::iiop::RequestIdHolder::operator<(const TIDorb::core::comm::iiop::RequestIdHolder& val)  const
// {
//   return _value < val._value;
// }




// bool TIDorb::core::comm::iiop::RequestIdHolder::operator>(const TIDorb::core::comm::iiop::RequestIdHolder& val)  const
// {
//   return _value > val._value;
// }




// bool TIDorb::core::comm::iiop::RequestIdHolder::operator==(TIDorb::core::comm::iiop::RequestId val) const 
// {
//   return _value == val;
// }




// bool TIDorb::core::comm::iiop::RequestIdHolder::operator< (TIDorb::core::comm::iiop::RequestId val)  const
// {
//   return _value < val;
// }




// bool TIDorb::core::comm::iiop::RequestIdHolder::operator>(TIDorb::core::comm::iiop::RequestId val)  const
// {
//   return _value > val;
// }



	
// char* TIDorb::core::comm::iiop::RequestIdHolder::toString() const
// {
//   TIDorb::util::StringBuffer os;
//   os << "RequestId: " <<_value;
//   return CORBA::string_dup(os.str().data());
// }
