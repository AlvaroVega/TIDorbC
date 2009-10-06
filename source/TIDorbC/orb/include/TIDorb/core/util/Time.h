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
