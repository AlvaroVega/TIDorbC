//////////////////////////////////////////////////////////////////////////////////
//
// File:        IORAddressingInfo.C
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

#include "TIDorb/core/comm/iiop/IORAddressingInfo.h"




TIDorb::core::comm::iiop::ProfileIIOP* TIDorb::core::comm::iiop::IORAddressingInfo::get_ProfileIIOP()
{
        if(_selected_profile_index == -1)
                return NULL;
        else
                return dynamic_cast<ProfileIIOP*>(_ior.getIOR()->get_profile(_selected_profile_index));
}




TIDorb::core::comm::iiop::IORAddressingInfo::IORAddressingInfo(CORBA::ULong selected_profile_index,
                                                               TIDorb::core::iop::IOR* ior)
{
	_ior = ior;
        _selected_profile_index  = selected_profile_index;
}




TIDorb::core::comm::iiop::IORAddressingInfo::IORAddressingInfo
        (const TIDorb::core::comm::iiop::IORAddressingInfo& info)
{
        _ior = info._ior.getIOR();
        _selected_profile_index = info._selected_profile_index;
}




TIDorb::core::comm::iiop::IORAddressingInfo* TIDorb::core::comm::iiop::IORAddressingInfo::read(TIDorb::core::cdr::CDRInputStream& input)
{
        CORBA::ULong index;

        input.read_ulong(index);

        TIDorb::core::iop::IOR* ior = new TIDorb::core::iop::IOR();

        ior->read(input);

        if (index >= ior->member_count())
                throw CORBA::MARSHAL();
                        //("Invalid selected Profile Number: " + index,
                        //0,CORBA::COMPLETED_NO);

        return new TIDorb::core::comm::iiop::IORAddressingInfo(index, ior);
}




void TIDorb::core::comm::iiop::IORAddressingInfo::write(TIDorb::core::cdr::CDROutputStream& output) const
{
        output.write_ulong(_selected_profile_index);
        _ior.getIOR()->write(output);
}

void TIDorb::core::comm::iiop::IORAddressingInfo::write
    (TIDorb::core::cdr::CDROutputStream& output,
     CORBA::ULong selected_profile_index, 
     const TIDorb::core::iop::IOR& ior)
{
        output.write_ulong(selected_profile_index);
        ior.write(output);
}
