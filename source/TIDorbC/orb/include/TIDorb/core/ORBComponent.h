/*//////////////////////////////////////////////////////////////////////////////////
//
// File:        ORBComponent.h
// Description: CORBA::Object implementation.
//
// Rel:         01.00
// Created:     September, 2002
// Author:      Juan A. Caceres Exposito      ( caceres@tid.es   )
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
///////////////////////////////////////////////////////////////////////////////////*/

#include "TIDorb/core.h"

#ifndef _TIDORB_core_ORBComponent_H_
#define _TIDORB_core_ORBComponent_H_ 1

/**
 * Base class for TIDorb Components. It defines the basic methods for accessing the ORB.
 * <p> Copyright 2000 Telef&oacute;nica I+D. Printed in Spain (Europe). All Rights Reserved.
 * @author Juan A. C&aacute;ceres
 * @version 1.0
 */

namespace TIDorb
{
namespace core
{

class ORBComponent: public virtual TIDThr::Monitor{

  /**
   * The ORB.
   */
  protected:
//MLG  
    //TIDORB_ref _orb;
    TIDORB* _orb;
//EMLG    

  public:
   ORBComponent(TIDORB* orb) throw (TIDThr::SystemException): _orb(orb){}
   virtual ~ORBComponent() throw (TIDThr::SystemException) {}

   TIDORB* orb() const { return _orb;}

};

}

}

#endif

