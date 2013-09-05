/////////////////////////////////////////////////////////////////////////
//
// File:        Counter.C
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

const CORBA::ULong TIDorb::core::util::Counter::RESERVED =
        TIDorb::core::util::ULong::MAX_VALUE;


TIDorb::core::util::Counter::Counter()
{
	value = 0;
}	

CORBA::ULong TIDorb::core::util::Counter::get_value()
{
	TIDThr::Synchronized synchro(recursive_mutex);	
	
	return value;
}

bool TIDorb::core::util::Counter::zero()
{
	TIDThr::Synchronized synchro(recursive_mutex);	
	
	return (value == 0);
}

bool TIDorb::core::util::Counter::non_zero()
{
	TIDThr::Synchronized synchro(recursive_mutex);	
	
	return (value != 0);
}

/**
 * Increments the counter 1 unity.
 * @return the new value of the counter.
 */
CORBA::ULong TIDorb::core::util::Counter::inc()
{
	TIDThr::Synchronized synchro(recursive_mutex);	
	
	value++;
	if (value == RESERVED) value++;
	
	return value;
}

/**
 * Decrements the counter 1 unity.
 * @return the new value of the counter.
 */
CORBA::ULong TIDorb::core::util::Counter::dec()
{
	TIDThr::Synchronized synchro(recursive_mutex);	
	
	if (value > 0)
		value--;
		
	return value;
}

/**
 * Increments the counter 1 unity.
 * @return the previous value of the counter.
 */
CORBA::ULong TIDorb::core::util::Counter::post_inc()
{
  TIDThr::Synchronized synchro(recursive_mutex);	
	CORBA::ULong post_value = value;
	value++;
	if (value == RESERVED) value++;
	
	return post_value;
}

/**
 * Decrements the counter 1 unity.
 * @return the previous value of the counter.
 */

CORBA::ULong TIDorb::core::util::Counter::post_dec()
{
	TIDThr::Synchronized synchro(recursive_mutex);	
	CORBA::ULong post_value = value;
		if (value > 0)
		value--;
		
	return post_value;
}
