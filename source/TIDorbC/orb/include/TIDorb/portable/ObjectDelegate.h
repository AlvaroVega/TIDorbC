/*/////////////////////////////////////////////////////////////////////////
//
// File:        ObjectDelegate.h
// Description: 
//              
// Rel:         01.00
// Created:     November, 2002
// Author:      Juan A. Caceres Exposito      ( caceres@tid.es   )
//
// Revised:
//
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
/////////////////////////////////////////////////////////////////////////*/

#include "TIDorb/portable.h"

#ifndef _TIDORB_PORTABLE_OBJECT_DELEGATE_H_
#define _TIDORB_PORTABLE_OBJECT_DELEGATE_H_

namespace TIDorb {
namespace portable {

class ObjectDelegate : public RefCounter {
  public:
    virtual ~ObjectDelegate() {}
       
    // ORB
    virtual TIDorb::portable::ORB* orb() = 0;
    
    // The reference has been created by the orb()
    
    virtual ::CORBA::Boolean is_local() = 0; 
    
     // Object existence test
    
    virtual ::CORBA::Boolean non_existent() = 0;     

  protected:
    ObjectDelegate(){};
};
}
}

#endif
