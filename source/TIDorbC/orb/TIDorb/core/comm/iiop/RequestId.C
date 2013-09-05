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
