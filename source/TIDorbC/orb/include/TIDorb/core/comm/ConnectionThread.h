///////////////////////////////////////////////////////////////////////////
//
// File:        CommunicationLayer.h
// Description: 
// Rel:         01.00
// Created:     May, 2001
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

#include "TIDorb/core/comm.h"

#ifndef _TIDORB_core_comm_ConnectionThread_H_
#define _TIDORB_core_comm_ConnectionThread_H_ 1

 
/**
 * Thread that receives messages in the connection.
 * 
 * <p> Copyright 2000 Telef&oacute;nica I+D. Printed in Spain (Europe). All Rights Reserved.
 *
 * @author Juan A. C&aacute;ceres
 * @version 1.0
 */

namespace TIDorb {
namespace core {
namespace comm {

class ConnectionThread : public TIDThr::Thread
{
    Connection_ref connection;

  public:
    ConnectionThread (Connection* conn, TIDThr::ThreadGroup* group)
      throw (TIDThr::SystemException);
    
    virtual ~ConnectionThread() throw (TIDThr::SystemException) {}

    void run();
};

}
}
}

#endif
