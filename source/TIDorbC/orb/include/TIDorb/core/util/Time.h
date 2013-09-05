/*//////////////////////////////////////////////////////////////////////////////////
//
// File:        Time.h
// Description: 
//
// Rel:         01.00
// Created:     
// Author:      
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
///////////////////////////////////////////////////////////////////////////////////*/

#ifndef __TIDorb_core_util_Time_H_
#define __TIDorb_core_util_Time_H_

#include "CORBA.h"
#include "TimeBase.h"
#include <sys/time.h>
#include <time.h>

namespace TIDorb {
namespace core {
namespace util {
  


class Time
{
public:
  /*  
   *  Difference between the CORBA Epoch (Gregorian Calendar) and the Unix Epoch: 
   *  the time from 1582/10/15 00:00 until 1970/01/01 00:00 in 100 ns units.
   */
  
  static CORBA::ULongLong UNIX_OFFSET; 
  
  /**
   *  1/1/1970 00:00 time in Gregorian Calendar
   */
  static CORBA::ULongLong GREGORIAN_CUTOVER;
  
  /**
   * Converts milliseconds to TimeBase::TimeT units (1 = 100 ns.)
   * @param timeInMillis
   * @return
   */
  
  static CORBA::ULongLong toTimeT(CORBA::ULongLong timeInMilliseconds);
  
  /**
   * Converts TimeBase::TimeT units (1 = 100 ns.) to milliseconds 
   * @param timeInMillis
   * @return
   */
  
  static CORBA::ULongLong toTimeInMillis(CORBA::ULongLong timeT);
  
  /**
   * Gets the current time in millisecons 
   * @return the value in millisecons
   */
  
  static CORBA::ULongLong currentTimeMillis();
  
  /**
   * Gets the current time in millisecons from 15th October 1582 00:00
   * @return the value in millisecons
   */
  
  static CORBA::ULongLong currentUtcTimeMillis();
  
  /**
   * Gets the current time in UtcT from 15th October 1582 00:00
   * @return the value in millisecons
   */
  
  static TimeBase::UtcT currentUtcT();
};


} //util
} //core
} //TIDorb

#endif
