/*/////////////////////////////////////////////////////////////////////////
//
// File:        Runnable.h
// Description: This file contains the Runnable abstract class declaration.
//             
// Rel:         01.00
// Created:     September, 2001
// Author:      Juan A. Caceres Exposito      ( caceres@tid.es   )
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
/////////////////////////////////////////////////////////////////////////*/

#include "TIDThr.h"

#ifndef __TID_THR_RUNNABLE__
#define __TID_THR_RUNNABLE__ 

namespace TIDThr {

class Runnable : public Monitor {
  public:
    virtual void run() = 0;
};
}

#endif
