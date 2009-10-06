//////////////////////////////////////////////////////////////////////////////////
//
// File:        TaggedProfile.h
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

#ifndef _TIDORB_CORE_IOP_TAGGEDPROFILE_H_
#define _TIDORB_CORE_IOP_TAGGEDPROFILE_H_

namespace TIDorb {
namespace core {
namespace iop {

class TaggedProfile : public TIDThr::RecursiveMutex
{
public:
        CORBA::ULong _tag;

        TaggedProfile(CORBA::ULong tag) throw (TIDThr::SystemException);
        virtual ~TaggedProfile() throw (TIDThr::SystemException){};

	virtual bool operator== (const TIDorb::core::iop::TaggedProfile& other) const = 0;

        void write(TIDorb::core::cdr::CDROutputStream& output) const;

        virtual void partial_write(TIDorb::core::cdr::CDROutputStream& output) const = 0;
        virtual void partial_read(TIDorb::core::cdr::CDRInputStream& input) = 0;

        static void skip(TIDorb::core::cdr::CDRInputStream& input);


};
}// iop
}// core
}// TIDorb
#endif

