///////////////////////////////////////////////////////////////////////////
//
// File:        TIDThr_Counter.C
// Description: This file contains the Counter class methods
// Rel:         01.00
// Created:     May, 2001
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
///////////////////////////////////////////////////////////////////////////

#include "TIDThr.h"

namespace TIDThr {

size_t
Counter::inc()
    throw (IllegalMutexStateException, 
	       SystemException)
{
    Synchronized synchro(*this);
    return ++m_count;
}

size_t
Counter::dec()
    throw (IllegalMutexStateException, 
	       SystemException)
{
    Synchronized synchro(*this);

    if((--m_count) == 0) {
        notifyAll();
    }

    return m_count;
}

void
Counter::waitForZero()
    throw (IllegalMutexStateException,
           InterruptedException,
           SystemException)
{
    Synchronized synchro(*this);
  
    while(m_count > 0) {
        wait();
    }
}

} // namespace TIDThr 
