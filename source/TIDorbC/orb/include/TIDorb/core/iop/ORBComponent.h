//////////////////////////////////////////////////////////////////////////////////
//
// File:        ORBComponent.h
// Description: 
//
// Rel:         01.00
// Created:     September, 2002
// Author:      Juan A. Caceres Exposito      ( caceres@tid.es   )
//
// Revised:
//
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Righ Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
///////////////////////////////////////////////////////////////////////////////////

#include "TIDorb/core/iop.h"

#ifndef _TIDORB_CORE_IOP_ORBCOMPONENT_H_
#define _TIDORB_CORE_IOP_ORBCOMPONENT_H_

namespace TIDorb {
namespace core {
namespace iop {

class ORBComponent : public virtual TIDorb::core::iop::TaggedComponent
{
private:
	char* _str;

public:
	CORBA::ULong orb_type;

	ORBComponent() throw (TIDThr::SystemException);
	
	ORBComponent(const ORBComponent& v) throw (TIDThr::SystemException);	
	
	ORBComponent(CORBA::ULong type) throw (TIDThr::SystemException);

	~ORBComponent() throw (TIDThr::SystemException);
	
	void write(TIDorb::core::cdr::CDROutputStream& out) const;
	
	void partial_read(TIDorb::core::cdr::CDRInputStream& input);

	const char* toString() const;
};
}// iop
}// core
}// TIDorb
#endif
