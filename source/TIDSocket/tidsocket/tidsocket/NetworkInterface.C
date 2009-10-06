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
// Copyright 2001 Telefonica, I+D. Printed in Spain (Europe). All Rights
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
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
