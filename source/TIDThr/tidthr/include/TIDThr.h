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
// Copyright 2001 Telefonica, I+D. Printed in Spain (Europe). All Rights
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
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
