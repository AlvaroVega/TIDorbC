///////////////////////////////////////////////////////////////////////////
//
// File:        TIDSocket_NetworkInterface.C
// Description: This file contains the NetworkInterface class methods
// Rel:         01.00
// Created:     May, 2001
// Author:      Pablo Rodriguez Archilla      ( pra@tid.es   )
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
///////////////////////////////////////////////////////////////////////////

#include "TIDSocket.h"




/*
   TODO: NetworkInterface not fully implemented.
   By now, a NetworkInterface is equivalent to a single InetAddress
*/




namespace TIDSocket
{




//
// Destructor
//
NetworkInterface::~NetworkInterface()
    throw()
{
}




//
// Assign operator
//
NetworkInterface& NetworkInterface::operator= (const NetworkInterface& netIf)
    throw()
{
    _ip_list[0] = netIf._ip_list[0];
    return *this;
}
    



//
// Equals operator
//
bool NetworkInterface::operator== (const NetworkInterface& netIf)
    throw()
{
    return (_ip_list[0] == netIf._ip_list[0]);
}




//
// getDisplayName()
//
char* NetworkInterface::getDisplayName()
    throw()
{
    return getName();
}     




//
// getInetAddresses()
//
const InetAddressList& NetworkInterface::getInetAddresses() const
    throw()
{
    return _ip_list;
}




//
// getName()
//
char* NetworkInterface::getName()
    throw()
{
    return _ip_list[0].getHostAddress();
}




//
// static getByInetAddress()
//
NetworkInterface* NetworkInterface::getByInetAddress(const InetAddress& addr)
    throw(SocketException)
{
    NetworkInterface* netIf = new NetworkInterface;
    netIf->_ip_list.push_back(addr);

    return netIf;
}




//
// static getByName()
//
NetworkInterface* NetworkInterface::getByName(const char* name)
    throw(SocketException)
{
    throw SocketException("getByName() not implemented");
}




//
// static getNetworkInterfaces()
//
NetworkInterfaceList* NetworkInterface::getNetworkInterfaces()
    throw(SocketException)
{
    throw SocketException("getNetworkInterfaces() not implemented");
}
    



} //namespace TIDSocket




//
// operator<<
//
ostream& operator<< (ostream& os, const TIDSocket::NetworkInterface& netIf)
{
    os << netIf.getInetAddresses()[0];
    return os;
}
