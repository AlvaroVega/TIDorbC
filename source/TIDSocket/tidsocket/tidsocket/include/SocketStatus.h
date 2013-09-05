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
