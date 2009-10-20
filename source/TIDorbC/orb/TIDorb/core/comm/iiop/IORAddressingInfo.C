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
