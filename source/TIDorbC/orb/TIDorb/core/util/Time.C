/////////////////////////////////////////////////////////////////////////
//
// File:        Time.C
// Description:
//              
// Rel:         01.00
// Created:     November, 2002
//
// Revised:
//
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Right
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
/////////////////////////////////////////////////////////////////////////

#include "TIDorb/core/util.h"

CORBA::ULongLong TIDorb::core::util::Time::UNIX_OFFSET = 122192928000000000ULL;

CORBA::ULongLong TIDorb::core::util::Time::GREGORIAN_CUTOVER = 12219292800000ULL;


CORBA::ULongLong TIDorb::core::util::Time::toTimeT(CORBA::ULongLong timeInMilliseconds)
{
  return (timeInMilliseconds * 10000);
}


CORBA::ULongLong TIDorb::core::util::Time::toTimeInMillis(CORBA::ULongLong timeT)
{
  return (timeT / 10000);
}


CORBA::ULongLong TIDorb::core::util::Time::currentTimeMillis()
{
  struct timeval the_time;
  gettimeofday(&the_time, NULL); // obtain Utc

  return ((the_time.tv_sec * 1000ULL) + (the_time.tv_usec / 1000ULL));
}


CORBA::ULongLong TIDorb::core::util::Time::currentUtcTimeMillis()
{
  return toTimeInMillis(toTimeT(currentTimeMillis()) + UNIX_OFFSET);
}


TimeBase::UtcT TIDorb::core::util::Time::currentUtcT()
{
  TimeBase::UtcT utc;
  time_t unix_time = currentTimeMillis();
  utc.time = (unix_time * 10000) + TIDorb::core::util::Time::UNIX_OFFSET;

  // Unix Time is always UTC -> no time zone offset
  utc.tdf = 0;
        
  // Nothing reasonable to put here
  utc.inacchi = 0;
  utc.inacclo = 0;

  return utc;
}
