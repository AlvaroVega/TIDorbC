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
