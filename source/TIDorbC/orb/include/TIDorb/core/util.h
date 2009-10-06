//////////////////////////////////////////////////////////////////////////////////
//
// File:        util.h
// Description: 
//
// Rel:         01.00
// Created:     September, 2002
// Author:      Juan A. Caceres Exposito      ( caceres@tid.es   )
//
// Revised:
//
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
///////////////////////////////////////////////////////////////////////////////////

#include "TIDorb/core.h"

#ifndef _TIDORB_CORE_UTIL_H_
#define _TIDORB_CORE_UTIL_H_

namespace TIDorb {
namespace core {
namespace util {
	
	class BooleanHolder;
	class CharHolder;
	class DoubleHolder;
	class FloatHolder;
	class LongDoubleHolder;
	class LongHolder;
	class LongLongHolder;
	class OctetHolder;
	class ShortHolder;
	class StringHolder;
	class ULongHolder;
	class ULongLongHolder;
	class UShortHolder;
	class WCharHolder;
	class WStringHolder;
	class FixedHolder;
	class AnyHolder;
	class ObjectHolder;
	class ValueHolder;
	class TypeCodeHolder;
	class SystemExceptionHolder;
	class Counter;
	class RemovableObject;
	class OperationCompletion;
	struct OnlyOneThreadCanWait;

	class Octet;
	class Short;
	class UShort;
	class Long;
	class ULong;
	class LongLong;
	class ULongLong;
	class Float;
	class Double;
	class LongDouble;

    class Time;
}
}
}

#include "TIDorb/core/util/InitialReference.h"

#include "TIDorb/core/util/URL.h"
#include "TIDorb/core/util/Corbaname.h"

#include "TIDorb/core/util/BooleanHolder.h"
#include "TIDorb/core/util/CharHolder.h"
#include "TIDorb/core/util/DoubleHolder.h"
#include "TIDorb/core/util/FloatHolder.h"
#include "TIDorb/core/util/LongDoubleHolder.h"
#include "TIDorb/core/util/LongHolder.h"
#include "TIDorb/core/util/LongLongHolder.h"
#include "TIDorb/core/util/OctetHolder.h"
#include "TIDorb/core/util/ShortHolder.h"
#include "TIDorb/core/util/StringHolder.h"
#include "TIDorb/core/util/ULongHolder.h"
#include "TIDorb/core/util/ULongLongHolder.h"
#include "TIDorb/core/util/UShortHolder.h"
#include "TIDorb/core/util/WCharHolder.h"
#include "TIDorb/core/util/WStringHolder.h"
#include "TIDorb/core/util/FixedHolder.h"
#include "TIDorb/core/util/AnyHolder.h"
#include "TIDorb/core/util/ObjectHolder.h"
#include "TIDorb/core/util/ValueHolder.h"
#include "TIDorb/core/util/OnlyOneThreadCanWait.h"
#include "TIDorb/core/util/OperationCompletion.h"
#include "TIDorb/core/util/TypeCodeHolder.h"
#include "TIDorb/core/util/Counter.h"

#include "TIDorb/core/util/RemovableObject.h"
#include "TIDorb/core/util/UseTable.h"


#include "TIDorb/core/util/CORBAException.h"
#include "TIDorb/core/util/exceptions.h"
#include "TIDorb/core/util/SystemExceptionHolder.h"

#include "TIDorb/core/util/Octet.h"
#include "TIDorb/core/util/Short.h"
#include "TIDorb/core/util/UShort.h"
#include "TIDorb/core/util/Long.h"
#include "TIDorb/core/util/ULong.h"
#include "TIDorb/core/util/LongLong.h"
#include "TIDorb/core/util/ULongLong.h"
#include "TIDorb/core/util/Float.h"
#include "TIDorb/core/util/Double.h"
#include "TIDorb/core/util/LongDouble.h"

#include "TIDorb/core/util/Time.h"

#endif
