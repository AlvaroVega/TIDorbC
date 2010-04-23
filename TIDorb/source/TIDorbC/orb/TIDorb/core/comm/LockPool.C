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
///////////////////////////////////////////////////////////////////////////////////

#include "TIDorb/core.h"

namespace TIDorb {
namespace core {
namespace comm {
    
Lock* LockPool::get_lock()
{
    TIDThr::Synchronized synchro(*this);

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
        TIDThr::Synchronized synchro(*this);    
        locks.push(lock);  
    }    
}

} // namespace
}
}
