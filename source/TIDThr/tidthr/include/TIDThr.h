/*//////////////////////////////////////////////////////////////////////////
//
// File:        TIDThr.h
// Description: This file contains the includes of the TIDThr declarations.
//             
// Rel:         01.00
// Created:     September, 2001
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
//////////////////////////////////////////////////////////////////////////*/

#ifndef __TID_THR__
#define __TID_THR__


#include "TIDThr/config.h"
#include "TIDThr/ErrnoStr.h"
#include "TIDThr/Exception.h"
#include "TIDThr/RefCounter.h"
#include "TIDThr/HandleT.h"

namespace TIDThr {
  class Mutex;
  typedef HandleT<Mutex> MutexHandle;
  class RecursiveMutex;
  typedef HandleT<RecursiveMutex> RecursiveMutexHandle;
  class Condition;
  typedef HandleT<Condition> ConditionHandle;
  class Monitor;
  typedef HandleT<Monitor> MonitorHandle;
  class Counter;
  class Key;
  typedef HandleT<Key> KeyHandle;
  class Runnable;
  typedef HandleT<Runnable> RunnableHandle;
  class ThreadId;
  typedef HandleT<ThreadId> ThreadIdHandle;
  class Thread;
  typedef HandleT<Thread> ThreadHandle;
  class ThreadGroupId;
  typedef HandleT<ThreadGroupId> ThreadGroupIdHandle;
  class ThreadGroup;
  typedef HandleT<ThreadGroup> ThreadGroupHandle;

  typedef int ChildId;
}

#include "TIDThr/MutexAttr.h"
#include "TIDThr/Mutex.h"
#include "TIDThr/RecursiveMutex.h"
#include "TIDThr/Condition.h"
#include "TIDThr/Monitor.h"
#include "TIDThr/Synchronized.h"
#include "TIDThr/Runnable.h"
#include "TIDThr/Key.h"
#include "TIDThr/Counter.h"
#include "TIDThr/ThreadId.h"      
#include "TIDThr/Thread.h"
#include "TIDThr/ThreadGroupId.h"
#include "TIDThr/ThreadGroup.h"


#endif
