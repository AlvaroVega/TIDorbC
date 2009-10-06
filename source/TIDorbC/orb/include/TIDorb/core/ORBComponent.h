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
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
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

