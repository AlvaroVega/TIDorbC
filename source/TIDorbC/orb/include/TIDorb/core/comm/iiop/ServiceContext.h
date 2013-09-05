/* -----------------------------------------------------------------------------

 File: TIDorb/core/comm/iiop/ServiceContext.h

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

#include "TIDorb/core/comm/iiop.h"

#ifndef _TIDorb_core_comm_iiop_SERVICECONTEXT_H_
#define _TIDorb_core_comm_iiop_SERVICECONTEXT_H_

namespace TIDorb {
namespace core {
namespace comm {
namespace iiop {

class ServiceContext: public virtual TIDThr::RefCounter {
public:
	
	CORBA::ULong _context_id;
	
  TIDorb::core::cdr::CDRInputStream* _context_data;
  
	ServiceContext(CORBA::ULong context_id) throw (TIDThr::SystemException);
	
	ServiceContext(CORBA::ULong context_id, TIDorb::core::cdr::CDRInputStream* context_data)
	 throw (TIDThr::SystemException);
	
	virtual void write(TIDorb::core::cdr::CDROutputStream& output) const;
		
	virtual void partial_read(TIDorb::core::cdr::CDRInputStream& input);
	
	static void skip(TIDorb::core::cdr::CDRInputStream& input);
	
	static void partial_skip(TIDorb::core::cdr::CDRInputStream& input);
	
	~ServiceContext()  throw (TIDThr::SystemException);
};
}// iiop
}// comm
}// core
}// TIDorb
#endif
