///////////////////////////////////////////////////////////////////////////
//
// File:        Monitor.C
// Description: This file contains the Monitor class methods
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

Monitor::Monitor(const pthread_condattr_t *cond_attr)
      throw (SystemException) : m_condition(cond_attr)
{}

void
Monitor::wait()
    throw (IllegalMonitorStateException,
           InterruptedException,
           SystemException)
{
    try {
        m_condition.wait((RecursiveMutex&)*this);
    } catch (const InterruptedException& ie) {
        throw ie;
    } catch (const SystemException &se) {
        throw se;
    } catch (const Exception &e) {
        throw IllegalMonitorStateException();
    }
};

void
Monitor::wait(time_t millisecons)
  throw (IllegalMonitorStateException,
         IllegalArgumentException,
         InterruptedException,
         SystemException)
{
    wait(millisecons,0);
}

void
Monitor::wait(time_t millisecons, time_t nanosecs)
    throw (IllegalMonitorStateException,
           IllegalArgumentException,
           InterruptedException,
           SystemException)
{
    try {
        m_condition.wait((RecursiveMutex&)*this, millisecons, nanosecs);
  } catch (const InterruptedException& ie) {
    throw ie;
  } catch (const IllegalArgumentException& iae) {
    throw iae;
  } catch (const SystemException &se) {
    throw se;
  } catch (const Exception &e) {
    throw IllegalMonitorStateException();
  }
}

void
Monitor::notify()
    throw (IllegalMonitorStateException,
           SystemException)
{
    try{
        m_condition.signal();
    } catch(const IllegalConditionStateException &ioe) {
        throw IllegalMonitorStateException();
    } catch (const SystemException &se) {
        throw se;
    } 
}

void
Monitor::notifyAll()
    throw (IllegalMonitorStateException,
           SystemException)
{
    try {
        m_condition.broadcast();
    } catch(const IllegalConditionStateException &ioe) {
        throw IllegalMonitorStateException();
    } catch (const SystemException &se) {
        throw se;
    } 
}

} // namespace TIDThr 
