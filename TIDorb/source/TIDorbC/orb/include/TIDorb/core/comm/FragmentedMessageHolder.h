/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/FragmentedMessageHolder.h
  
 Revisions:
 
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
 ------------------------------------------------------------------------------ */


#include "TIDorb/core/comm.h"

#ifndef _TIDORB_core_comm_FragmentedMessageHolder_H_
#define _TIDORB_core_comm_FragmentedMessageHolder_H_ 1


/**
 * Adapter class for maintaining an uncompleted fragmented message in a 
 * <code>UseTable</code>.
 * @see es.tid.TIDorbj.core.util.UseTable
 * <p> Copyright 2000 Telef&oacute;nica I+D. Printed in Spain (Europe). All Rights Reserved.
 *
 * @autor Juan A. C&aacute;ceres
 * @version 1.0
 */

#include "TIDorb/core/util.h"

namespace TIDorb {
  
namespace core {
  
namespace comm {
   
class FragmentedMessageHolder : public TIDorb::core::util::RemovableObject {
    
  protected:
  
  TIDorb::core::comm::iiop::GIOPFragmentedMessage* message;

  public: 
  
  FragmentedMessageHolder(TIDorb::core::comm::iiop::GIOPFragmentedMessage* msg);
  virtual ~FragmentedMessageHolder();
  
  /**
   * Gets the message
   */
   
  TIDorb::core::comm::iiop::GIOPFragmentedMessage* get_message()
  {
    return message;
  }
  
  /**
   * Gets and set to NULL the message, next consume_message will be NULL
   */
   
  TIDorb::core::comm::iiop::GIOPFragmentedMessage* consume_message();
  
  bool canBeRemoved();
};

}

}

}
#endif

