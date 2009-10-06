/*/////////////////////////////////////////////////////////////////////////
//
// File:        ORB.h
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

#include "CORBA.h"

#ifndef _TIDorb_portable_ORB_H_
#define _TIDorb_portable_ORB_H_

namespace TIDorb {
namespace portable {

class ORB : public CORBA::ORB
{
  public:

    virtual ~ORB() {}

    virtual ::TIDorb::portable::OutputStream* create_output_stream() const = 0;
    virtual CORBA::Any* create_any() const = 0;
    

  protected:
    ORB(){}


}; // class TIDORB

}  // namespace portable
}  // namespace TIDorb

#endif
