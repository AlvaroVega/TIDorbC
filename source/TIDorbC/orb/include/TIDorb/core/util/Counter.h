/*//////////////////////////////////////////////////////////////////////////////////
//
// File:        Counter.h
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

#include "TIDorb/core/util.h"

#ifndef _TIDORB_core_util_Counter_H_
#define _TIDORB_core_util_Counter_H_ 1

/* -----------------------------------------------------------------------------

 File: TIDorbj/core/util/Counter.h
  
 Revisions:
 
 Copyright 2003 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
 The copyright to the software program(s) is property of Telefonica I+D.
 The program(s) may be used and or copied only with the express written
 consent of Telefonica I+D or in acordance with the terms and conditions
 stipulated in the agreement/contract under which the program(s) have
 been supplied.   
 ------------------------------------------------------------------------------ */

/**
 * Simple syncrhonized counter.
 * <p> Copyright 2000 Telef&oacute;nica I+D. Printed in Spain (Europe). All Rights Reserved.
 *
 * @autor Juan A. C&aacute;ceres
 * @version 0.1
 */

namespace TIDorb {
namespace core {
namespace util {
	
class Counter {
	
    private:
	 CORBA::ULong value;
     TIDThr::RecursiveMutex recursive_mutex;	 
	
	public:
	 static const CORBA::ULong RESERVED; // = (CORBA::ULong) -1;

	 Counter();
	 CORBA::ULong get_value();
	 bool zero();
	 bool non_zero();
	 
	 /**
	  * Increments the counter 1 unity.
	  * @return the new value of the counter.
	  */
	 CORBA::ULong inc();

	/**
	 * Decrements the counter 1 unity.
	 * @return the new value of the counter.
	 */
	 CORBA::ULong dec();

	/**
	 * Increments the counter 1 unity.
	 * @return the previous value of the counter.
	 */
	 CORBA::ULong post_inc();

	/**
	 * Decrements the counter 1 unity.
	 * @return the previous value of the counter.
	 */

	 CORBA::ULong post_dec();
};

}

}

}

#endif
	
	
