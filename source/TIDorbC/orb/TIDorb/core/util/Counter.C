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
