///////////////////////////////////////////////////////////////////////////
//
// File:        TIDSocket_include/SocketStatus.h
// Description: 
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

#ifndef __TID_SOCKET_SOCKET_STATUS_H__
#define __TID_SOCKET_SOCKET_STATUS_H__


//
// Flags de estado de un socket
//
#define TID_SOCKET_STATUS_UNSPECIFIED       0x0000L
#define TID_SOCKET_STATUS_CREATED           0x0001L
#define TID_SOCKET_STATUS_BOUND             0x0002L
#define TID_SOCKET_STATUS_CONNECTED         0x0004L
#define TID_SOCKET_STATUS_INPUT_SHUTDOWN    0x0008L
#define TID_SOCKET_STATUS_OUTPUT_SHUTDOWN   0x0010L
#define TID_SOCKET_STATUS_CLOSED            0x0020L


#endif //__TID_SOCKET_SOCKET_STATUS_H__
