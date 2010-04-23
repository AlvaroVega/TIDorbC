///////////////////////////////////////////////////////////////////////////
//
// File:        TIDSocket_SSLException.h
// Description: This file contains the SSLException class methods
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


#ifndef __TID_SOCKET_SSL_EXCEPTION_H__
#define __TID_SOCKET_SSL_EXCEPTION_H__



namespace TIDSocket
{


class SSLException : public IOException
{
public:

  SSLException(const char* msg = "", int errnum = 0)
    throw() : IOException(msg, errnum) {}

  virtual ~SSLException()
    throw() {}

  const char* getName() const
    throw() { return "TIDSocket::SSLException"; }

};


} //namespace TIDSocket




#endif //__TID_SOCKET_SSL_EXCEPTION_H__
