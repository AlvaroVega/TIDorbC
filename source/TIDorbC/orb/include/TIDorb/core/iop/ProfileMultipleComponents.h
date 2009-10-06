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
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Righ Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
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

        //synchronized void extract_members(); //ATENCION - MCPG
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
