/////////////////////////////////////////////////////////////////////////
//
// File:        TaggedProfile.C
// Description:
//              
// Rel:         01.00
// Created:     November, 2002
//
// Revised:    
//
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
/////////////////////////////////////////////////////////////////////////

#include "TIDorb/core/iop/TaggedProfile.h"




TIDorb::core::iop::TaggedProfile::TaggedProfile(CORBA::ULong tag)
        throw (TIDThr::SystemException)
{
        _tag = tag;
}




void TIDorb::core::iop::TaggedProfile::write(TIDorb::core::cdr::CDROutputStream& output) const
{
        // write tag
        output.write_ulong(_tag);

        // write profile data
        partial_write(output);
}




void TIDorb::core::iop::TaggedProfile::skip(TIDorb::core::cdr::CDRInputStream& input)
{
        // skip tag
        input.skip_ulong();

        // skip encapsulation
        input.skip_encapsulation();
}
