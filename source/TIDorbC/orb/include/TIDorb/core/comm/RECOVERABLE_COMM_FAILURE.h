/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/RECOVERABLE_COMM_FAILURE
  
 Revisions:
 
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
 ------------------------------------------------------------------------------ */

#include "TIDorb/core/comm.h"

#ifndef _TIDORB_core_comm_RECOVERABLE_COMM_FAILURE_H_
#define _TIDORB_core_comm_RECOVERABLE_COMM_FAILURE_H_ 1

 
/**
 * Failure thrown in connection that can be recovered for resending requests.
 * 
 * <p> Copyright 2000 Telef&oacute;nica I+D. Printed in Spain (Europe). All Rights Reserved.
 *
 * @autor Juan A. C&aacute;ceres
 * @version 1.0
 */

#include <exception>

#ifdef TIDORB_HAVE_NAMESPACE_STD
  using namespace std;
#endif

#ifdef __SUNPRO_LIBRARY_IOSTREAM
  using std::exception;
#endif

namespace TIDorb {
namespace core {
namespace comm {

class RECOVERABLE_COMM_FAILURE : public virtual exception {
	
	public:
	 CORBA::COMM_FAILURE comm_failure;
	
  	 RECOVERABLE_COMM_FAILURE(const CORBA::COMM_FAILURE& failure) throw();
	 ~RECOVERABLE_COMM_FAILURE() throw() {};

	 const char* what() const throw();
};

}
}
}
#endif

