//////////////////////////////////////////////////////////////////////////////////
//
// File:        LockPool.C
// Description: 
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
///////////////////////////////////////////////////////////////////////////////////

#include "TIDorb/core.h"

namespace TIDorb {
namespace core {
namespace comm {
    
Lock* LockPool::get_lock()
{
    TIDThr::Synchronized (*this);
    
    Lock* lock = NULL;
    
    if(locks.size() > 0) {
        lock = locks.top();
        lock->recycle();
        locks.pop();
    } else {
        lock = new Lock();
    }
    
    return lock; 
}

void LockPool::put_lock(Lock* lock)
{
    lock->clear();
    {
        TIDThr::Synchronized (*this);    
        locks.push(lock);  
    }    
}

} // namespace
}
}
