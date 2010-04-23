///////////////////////////////////////////////////////////////////////////
//
// File:        TIDSocket_SSLPeerUnverifiedException.h
// Description: This file contains the SSLPeerUnverifiedException class methods
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


class SSLPeerUnverifiedException : public SSLException
{
public:

  SSLPeerUnverifiedException(const char* msg = "", int errnum = 0)
    throw() : SSLException(msg, errnum) {}

  virtual ~SSLPeerUnverifiedException()
    throw() {}

  const char* getName() const
    throw() { return "TIDSocket::SSLPeerUnverifiedException"; }

};


} //namespace TIDSocket




#endif //__TID_SOCKET_SSL_HANDSHAKE_EXCEPTION_H__
