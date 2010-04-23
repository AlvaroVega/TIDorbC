///////////////////////////////////////////////////////////////////////////
//
// File:        TIDSocket_SSLHandshakeException.h
// Description: This file contains the SSLHandshakeException class methods
// Rel:         01.00
// Created:     May, 2009
// Author:      Alvaro Vega Garcia      ( avega@tid.es   )
//
// Revised:
//
// Copyright 2009 Telefonica, I+D. Printed in Spain (Europe). All Rights
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
///////////////////////////////////////////////////////////////////////////

#include "TIDSocket.h"


#ifndef __TID_SOCKET_SSL_HANDSHAKE_EXCEPTION_H__
#define __TID_SOCKET_SSL_HANDSHAKE_EXCEPTION_H__



namespace TIDSocket
{


class SSLHandshakeException : public SSLException
{
public:

  SSLHandshakeException(const char* msg = "", int errnum = 0)
    throw() : SSLxception(msg, errnum) {}

  virtual ~SSLHandshakeException()
    throw() {}

  const char* getName() const
    throw() { return "TIDSocket::SSLHandshakeException"; }

};


} //namespace TIDSocket




#endif //__TID_SOCKET_SSL_HANDSHAKE_EXCEPTION_H__
