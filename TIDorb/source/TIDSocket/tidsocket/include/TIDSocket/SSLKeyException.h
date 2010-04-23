///////////////////////////////////////////////////////////////////////////
//
// File:        TIDSocket_SSLKeyException.h
// Description: This file contains the SSLKeyException class methods
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


#ifndef __TID_SOCKET_SSL_KEY_EXCEPTION_H__
#define __TID_SOCKET_SSL_KEY_EXCEPTION_H__



namespace TIDSocket
{


class SSLKeyException : public SSLException
{
public:

  SSLKeyException(const char* msg = "", int errnum = 0)
    throw() : SSLException(msg, errnum) {}

  virtual ~SSLKeyException()
    throw() {}

  const char* getName() const
    throw() { return "TIDSocket::SSLKeyException"; }

};


} //namespace TIDSocket




#endif //__TID_SOCKET_SSL_KEY_EXCEPTION_H__
