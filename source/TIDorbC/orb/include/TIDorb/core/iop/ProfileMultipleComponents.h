//////////////////////////////////////////////////////////////////////////////////
//
// File:        ProfileMultipleComponents.h
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

#include "TIDorb/core/iop.h"

#ifndef _TIDORB_CORE_COMM_IIOP_PROFILEMULTIPLECOMPONENTS_H_
#define _TIDORB_CORE_COMM_IIOP_PROFILEMULTIPLECOMPONENTS_H_

#include "TIDThr.h"

namespace TIDorb {
namespace core {
namespace iop {


// TODO: Is TAG_MULTIPLE_COMPONENTS a TaggedProfile or a TaggedComponent?
//       Check TaggedProfileReader and TaggedComponentReader


class ProfileMultipleComponents : public TIDorb::core::iop::TaggedProfile {

private:

        VectorTaggedComponent _components;

        TIDorb::core::cdr::CDRInputStream* _profile_data;

        bool _components_extracted;

protected:

        void extract_members();

public:

        ProfileMultipleComponents() throw (TIDThr::SystemException);

        ProfileMultipleComponents(const VectorTaggedComponent& tagged_components) throw (TIDThr::SystemException);

        ~ProfileMultipleComponents() throw (TIDThr::SystemException);

        const VectorTaggedComponent& getTaggedComponents() const;

	bool operator== (const TIDorb::core::iop::TaggedProfile& other) const;

        void partial_write(TIDorb::core::cdr::CDROutputStream& output) const;

        void partial_read(TIDorb::core::cdr::CDRInputStream& input);


};
}// iop
}// core
}// TIDorb
#endif
