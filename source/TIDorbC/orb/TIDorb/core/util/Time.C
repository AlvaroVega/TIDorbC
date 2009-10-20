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
